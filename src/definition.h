/*************************************************************************
 *
 * Copyright (C) 2014-2017 Barbara Geller & Ansel Sermersheim 
 * Copyright (C) 1997-2014 by Dimitri van Heesch.
 * All rights reserved.    
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation under the terms of the GNU General Public License version 2
 * is hereby granted. No representations are made about the suitability of
 * this software for any purpose. It is provided "as is" without express or
 * implied warranty. See the GNU General Public License for more details.
 *
 * Documents produced by DoxyPress are derivative works derived from the
 * input used in their production; they are not affected by this license.
 *
*************************************************************************/

#ifndef DEFINITION_H
#define DEFINITION_H

#include <QHash>
#include <QSharedPointer>
#include <QTextStream>
#include <QVector>

#include <doxy_shared.h>
#include <sortedlist.h>
#include <types.h>

class Definition_Private;
class FileDef;
class GroupDef;
class MemberSDict;
class MemberDef;
class OutputList;

struct ListItemInfo;
struct SectionInfo;

/** Data associated with a detailed description. */
struct DocInfo {
   QString doc;
   int line;
   QString file;

   DocInfo() : line(1) {};
};

/** Data associated with a brief description. */
struct BriefInfo {
   QString doc;
   QString tooltip;
   int line;
   QString file;

   BriefInfo() : line(1) {};
};

/** Abstract interface for a Definition or DefinitionList */
class DefinitionIntf : public EnableSharedFromThis 
{
 public:
   DefinitionIntf() { }
   virtual ~DefinitionIntf() { }

   /*! Types of derived classes */
   enum DefType {
      TypeClass      = 0,
      TypeFile       = 1,
      TypeNamespace  = 2,
      TypeMember     = 3,
      TypeGroup      = 4,
      TypePackage    = 5,
      TypePage       = 6,
      TypeDir        = 7,
      TypeSymbolList = 8
   };

   /*! Use this for dynamic inspection of the type of the derived class */
   virtual DefType definitionType() const = 0;
};

/** The common base class of all entity definitions found in the sources.
 *
 *  This can be a class or a member function, or a file, or a namespace, etc.
 *  Use definitionType() to find which type of definition this is.
 */
class Definition : public DefinitionIntf
{
 public:
  
   Definition(const QString &defFileName, int defLine, int defColumn, const QString &name, 
      const QString &b = QString(), const QString &d = QString(), bool isPhrase = true);
 
   virtual ~Definition();

   /*! Returns the name of the definition */
   const QString &name() const {
      return m_name;
   }

   /*! Returns the name of the definition as it appears in the output */
   virtual QString displayName(bool includeScope = true) const = 0;

   /*! Returns the local name without any scope qualifiers. */
   QString localName() const;

   /*! Returns the fully qualified name of this definition
    */
   virtual QString qualifiedName() const;

   /*! Returns the name of this definition as it appears in the glossary map
    */
   QString phraseName() const;

   /*! Returns the base file name (without extension) of this definition.
    *  as it is referenced to/written to disk.
    */
   virtual QString getOutputFileBase() const = 0;

   /*! Returns the anchor within a page where this item can be found */
   virtual QString anchor() const = 0;

   /*! Returns the name of the source listing of this definition. */
   virtual QString getSourceFileBase() const;

   /*! Returns the anchor of the source listing of this definition. */
   virtual QString getSourceAnchor() const;

   /*! Returns the detailed description of this definition */
   virtual QString documentation() const;

   /*! Returns the line number at which the detailed documentation was found. */
   int docLine() const;

   /*! Returns the file in which the detailed documentation block was found.
    *  This can differ from getDefFileName().
    */
   QString docFile() const;

   /*! Returns the brief description of this definition. This can include commands. */
   virtual QString briefDescription(bool abbreviate = false) const;

   /*! Returns a plain text version of the brief description suitable for use
    *  as a tool tip.
    */
   QString briefDescriptionAsTooltip()  const;

   /*! Returns the line number at which the brief description was found. */
   int briefLine() const;

   /*! Returns the documentation found inside the body of a member */
   QString inbodyDocumentation() const;

   /*! Returns the file in which the in body documentation was found */
   QString inbodyFile() const;

   /*! Returns the line at which the first in body documentation
       part was found */
   int inbodyLine() const;

   /*! Returns the file in which the brief description was found.
    *  This can differ from getDefFileName().
    */
   QString briefFile() const;

   /*! returns the file in which this definition was found */
   QString getDefFileName() const;

   /*! returns the extension of the file in which this definition was found */
   QString getDefFileExtension() const;

   /*! returns the line number at which the definition was found */
   int getDefLine() const {
      return m_defLine;
   }

   /*! returns the column number at which the definition was found */
   int getDefColumn() const {
      return m_defColumn;
   }

   /*! Returns true iff the definition is documented
    *  (which could be generated documentation)
    *  @see hasUserDocumentation()
    */
   virtual bool hasDocumentation() const;

   /*! Returns true iff the definition is documented by the user. */
   virtual bool hasUserDocumentation() const;

   /*! Returns true iff it is possible to link to this item within this
    *  project.
    */
   virtual bool isLinkableInProject() const = 0;

   /*! Returns true iff it is possible to link to this item. This can
    *  be a link to another project imported via a tag file.
    */
   virtual bool isLinkable() const = 0;

   /*! Returns true iff the name is part of this project and
    *  may appear in the output
    */
   virtual bool isVisibleInProject() const;

   /*! Returns true iff the name may appear in the output */
   virtual bool isVisible() const;

   /*! Returns true iff this item is supposed to be hidden from the output. */
   bool isHidden() const;

   /*! returns true if this entity was artificially introduced, for
    *  instance because it is used to show a template instantiation relation.
    */
   bool isArtificial() const;

   /*! If this definition was imported via a tag file, this function
    *  returns the tagfile for the external project. This can be
    *  translated into an external link target via Doxy_Globals::tagDestinationDict
    */
   virtual QString getReference() const;

   /*! Returns true if this definition is imported via a tag file. */
   virtual bool isReference() const;

   /*! Returns the first line of the body of this item (applicable to classes and
    *  functions).
    */
   int getStartBodyLine() const;

   /*! Returns the last line of the body of this item (applicable to classes and
    *  functions).
    */
   int getEndBodyLine() const;

   /*! Returns the file in which the body of this item is located or 0 if no
    *  body is available.
    */
   QSharedPointer<FileDef> getBodyDef() const;

   /** Returns the programming language this definition was written in. */
   SrcLangExt getLanguage() const;

   SortedList<QSharedPointer<GroupDef>> *partOfGroups() const;
   bool isLinkableViaGroup() const;

   virtual QSharedPointer<Definition> findInnerCompound(const QString &name);
   virtual QSharedPointer<Definition> getOuterScope() const;

   const MemberSDict &getReferencesMembers() const;
   const MemberSDict &getReferencedByMembers() const;

   bool hasSections() const;
   bool hasSources() const;

   /** returns true if this class has a brief description */
   bool hasBriefDescription() const;

   QString id() const;

   int getInputOrderId() {
      return m_inputOrderId;
   }   

   int getSortId() {
      return m_sortId;
   }   

   /*! Sets a new \a name for the definition */
   virtual void setName(const QString &name);

   /*! Sets a unique id for the definition. Used for libclang integration. */
   void setId(const QString &name);

   /*! Sets the documentation of this definition to \a d. */
   virtual void setDocumentation(const QString &d, const QString &docFile, int docLine, 
                  bool stripWhiteSpace = true, bool atTop = false);

   /*! Sets the brief description of this definition to \a b.
    *  A dot is added to the sentence if not available.
    */
   virtual void setBriefDescription(const QString &b, const QString &briefFile, int briefLine);

   /*! Set the documentation that was found inside the body of an item.
    *  If there was already some documentation set, the new documentation
    *  will be appended.
    */
   virtual void setInbodyDocumentation(const QString &d, const QString &docFile, int docLine);

   /*! Sets the tag file id via which this definition was imported. */
   void setReference(const QString &r);

   /*! Add the list of anchors that mark the sections that are found in the documentation.
    */
   void addSectionsToDefinition(const QVector<SectionInfo> &anchorList);

   // source references
   void setBodySegment(int bls, int ble);
   void setBodyDef(QSharedPointer<FileDef> fd);
   void addSourceReferencedBy(QSharedPointer<MemberDef>d);
   void addSourceReferences(QSharedPointer<MemberDef>d);

   void setRefItems(const QVector<ListItemInfo> &list);
   void mergeRefItems(QSharedPointer<Definition> d);
   const QVector<ListItemInfo> &getRefItems() const;
   QVector<ListItemInfo> &getRefItems();

   virtual void addInnerCompound(QSharedPointer<Definition> d) {};
   virtual void setOuterScope(QSharedPointer<Definition> d);
   virtual void setHidden(bool b);

   void setArtificial(bool b);
   void setLanguage(SrcLangExt lang);

   void setInputOrderId(int id) {
      m_inputOrderId = id;
   }   

   void setSortId(int id) {
      m_sortId = id;
   }   

   QString convertNameToFile(const QString &name, bool allowDots = false) const;

   void writeSourceDef(OutputList &ol, const QString &scopeName);
   void writeInlineCode(OutputList &ol, const QString &scopeName);
   void writeSourceRefs(OutputList &ol, const QString &scopeName);
   void writeSourceReffedBy(OutputList &ol, const QString &scopeName);
   void makePartOfGroup(QSharedPointer<GroupDef> gd);
   
   void writeNavigationPath(OutputList &ol) const;
   QString navigationPathAsString() const;

   virtual void writeQuickMemberLinks(OutputList &, QSharedPointer<MemberDef> md) const 
   {}

   virtual void writeSummaryLinks(OutputList &) 
   {}

   QString pathFragment() const;
   
   /*! Writes the documentation anchors of the definition to yhe Doxy_Globals::tagFile stream.
    */
   void writeDocAnchorsToTagFile(QTextStream &);
   void setLocalName(const QString &name);

   void addSectionsToIndex(bool addToNavIndex);
   void writeToc(OutputList &ol);

   virtual QString getHint() {
      return "";
   }

 protected:
   Definition(const Definition &d);
   virtual QString pathFragment_Internal() const;

 private:
   void addToMap(const QString &name);
 
   void setPhraseName(const QString &phrase);

   int  _getXRefListId(const QString &listName) const;
   void _writeSourceRefList(OutputList &ol, const QString &scopeName,const QString &text, 
                  const MemberSDict &members);

   void _setInbodyDocumentation(const QString &d, const QString &docFile, int docLine);
   bool _docsAlreadyAdded(const QString &doc, QString &sigList);

   Definition_Private *m_private; 
   QString m_name;

   bool m_isPhrase;

   QString m_phraseName;
   int m_defLine;
   int m_defColumn;

   int m_inputOrderId;
   int m_sortId;
};

#endif
