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

#ifndef STRINGMAP_H
#define STRINGMAP_H

#include <QList>
#include <QHash>

#include <typeinfo>
#include <algorithm>
#include <stdexcept>

#include <config.h>
#include <definition.h>
#include <types.h>

class ClassDef;
class DirDef;
class Example;
class FileNameList;
class FilePair;
class GroupDef;
class MemberDef;
class MemberGroup;
class MemberName;
class MemberNameInfo;
class NamespaceDef;
class OutputList;
class PageDef;
class SearchDefinitionList;
class SectionInfo;
class UseEntry;

struct IndexField;

// Ordered dictionary of elements of type T Internally uses a QMap<T>
class StringCompare {
   public:
      explicit StringCompare(Qt::CaseSensitivity sortCase) : m_sortCase_enum(sortCase) { }

      bool operator()(const QString &a, const QString &b) const {
         return a.compare(b, m_sortCase_enum) < 0;
      }

   private:
      Qt::CaseSensitivity m_sortCase_enum;
};

template<class T, class SC = StringCompare>
class StringMap
{

 private:
   QMap<QString, T, SC> m_dict;

 public:
   // create an ordered dictionary
   // indicate whether the keys should be sorted in a case sensitive way

   using iterator = typename QMap<QString, T, SC>::iterator;
   using const_iterator = typename QMap<QString, T, SC>::const_iterator;

   StringMap(SC compare) : m_dict(compare) {
   }

   StringMap(Qt::CaseSensitivity sortCase_enum = Qt::CaseSensitive) : m_dict(StringCompare(sortCase_enum)) {
   }

   virtual ~StringMap() {
   }

   iterator begin() {
      return m_dict.begin();
   }

   const_iterator begin() const {
      return m_dict.begin();
   }

   void clear() {
      m_dict.clear();
   }

   int count() const {
      return m_dict.count();
   }

   virtual int compareMapValues(const T &item1, const T &item2) const {

      if (item1 < item2) {
         return -1;
      }

      return 0;
   }

   iterator end() {
      return m_dict.end();
   }

   const_iterator end() const {
      return m_dict.end();
   }

   T find(const char *key) const {
      auto item = m_dict.find(key);

      if (item == m_dict.end()) {
         return T();
      }

      return item.value();
   }

   T find(const QByteArray &key) const {
      auto item = m_dict.find(key);

      if (item == m_dict.end()) {
         return T();
      }

      return item.value();
   }

   T find(const QString &key) const {
       auto item = m_dict.find(key);

      if (item == m_dict.end()) {
         return T();
      }

      return item.value();
   }

   void insert(const char *key, const T &d) {
      m_dict.insert(key, d);
   }

   void insert(QByteArray key, const T &d) {
      m_dict.insert(key, d);
   }

   void insert(QString key, const T &d) {
      m_dict.insert(key, d);
   }

   bool isEmpty() const {
      return m_dict.isEmpty();
   }

   /*! Remove an item from the dictionary */
   bool remove(const QString &key) {
      return m_dict.remove(key);
   }

   /*! Take an item out of the dictionary without deleting it */
   T *take(const char *key) {
      return m_dict.take(key);
   }

   T &operator[](const char *key) {
      return m_dict[key];
   }

   T operator[](const char *key) const {
      return m_dict[key];
   }

   T &operator[](const QString &key) {
      return m_dict[key];
   }

   T operator[](const QString &key) const {
      return m_dict[key];
   }

   T &operator[](const QByteArray &key) {
      return m_dict[key];
   }

   T operator[](const QByteArray &key) const {
      return m_dict[key];
   }

   class Iterator;         // first forward declare
   friend class Iterator;  // then make it a friend

   /*! Simple iterator for SDict. It iterates in the order in which the
    *  elements are stored.
    */
   class Iterator
   {
    public:
      /*! Create an iterator given the dictionary. */
      Iterator(const StringMap<T> &dict) {

         m_list = dict.m_dict.values();
         std::sort(m_list.begin(), m_list.end(), [&dict](const T &v1, const T &v2){ return dict.compareMapValues(v1, v2) < 0; } );

         m_li = m_list.begin();
      }

      /*! Destroys the dictionary */
      virtual ~Iterator() {
      }

      /*! Set the iterator to the first element in the list.
       *  \return The first compound, or zero if the list was empty.
       */
      T toFirst() {
         m_li = m_list.begin();

         if (m_li == m_list.end()) {
            return T();
         } else {
            return *m_li;
         }
      }

      /*! Set the iterator to the last element in the list.
       *  \return The first compound, or zero if the list was empty.
       */
      T toLast()  {
         if (m_list.isEmpty()) {
            m_li = m_list.end();
            return T();
         }

         m_li = m_list.end() - 1;
         return *m_li;
      }

      /*! Returns the current compound */
      T current() const {

         if (m_li == m_list.end()) {
            return T();
         } else {
            return *m_li;
         }
      }

      /*! Moves the iterator to the next element.
       *  \return the new "current" element, or zero if the iterator was
       *          already pointing at the last element.
       */
      void operator++() {
         ++m_li;
      }

      /*! Moves the iterator to the previous element.
       *  \return the new "current" element, or zero if the iterator was
       *          already pointing at the first element.
       */
      void operator--() {
         --m_li;
      }

    private:
      QList<T> m_list;
      typename QList<T>::iterator m_li;
   };

};


/** Ordered dictionary of elements of type T.
 *
 */
template<class T>
class LongMap
{
 private:
   QMap<long, T> m_dict;

 public:

   using const_iterator = typename QMap<long, T>::const_iterator;

   LongMap() {
   }

   /*! Destroys the dictionary */
   virtual ~LongMap() {
   }

   using iterator = typename QMap<long, T>::iterator;

   iterator begin() {
      return m_dict.begin();
   }

   const_iterator begin() const {
      return m_dict.begin();
   }

   void clear() {
      m_dict.clear();
   }

   virtual int compareMapValues(const T &item1, const T &item2) const {
      if (item1 < item2) {
         return -1;
      }

      return 0;
   }

   bool contains(long key) {
      return m_dict.contains(key);
   }

   int count() const {
      return m_dict.count();
   }

   iterator end() {
      return m_dict.end();
   }

   const_iterator end() const {
      return m_dict.end();
   }

   T find(long key) {
       auto item = m_dict.find(key);

      if (item == m_dict.end()) {
         return T();
      }

      return item.value();
   }

   void insert(long key, const T &d) {
      m_dict.insert(key, d);
   }

    bool remove(long key) {
      return m_dict.remove(key);
   }

   T &operator[](long key) {
      return m_dict[key];
   }

   T operator[](long key) const {
      return m_dict[key];
   }

   class Iterator;         // first forward declare
   friend class Iterator;  // then make it a friend

   /*! Simple iterator for SDict. It iterates in the order in which the
    *  elements are stored.
    */
   class Iterator
   {
    public:
      /*! Create an iterator given the dictionary. */
      Iterator(const LongMap<T> &dict) {

         m_list = dict.m_dict.values();
         std::sort(m_list.begin(), m_list.end(), [&dict](const T &v1, const T &v2){ return dict.compareMapValues(v1, v2) < 0; } );

          m_li = m_list.begin();
      }

      /*! Destroys the dictionary */
      virtual ~Iterator() {
      }

      /*! Set the iterator to the first element in the list.
       *  \return The first compound, or zero if the list was empty.
       */
      T toFirst() {
         m_li = m_list.begin();
         return *m_li;
      }

      /*! Set the iterator to the last element in the list.
       *  \return The first compound, or zero if the list was empty.
       */
      T toLast() {
         m_li = m_list.end() - 1;
         return *m_li;
      }

      /*! Returns the current compound */
      T current() const {
         return *m_li;
      }

      /*! Moves the iterator to the next element.
       *  \return the new "current" element, or zero if the iterator was
       *          already pointing at the last element.
       */
      void operator++() {
         ++m_li;
      }

      /*! Moves the iterator to the previous element.
       *  \return the new "current" element, or zero if the iterator was
       *          already pointing at the first element.
       */
      void operator--() {
         --m_li;
      }

    private:
      QList<T> m_list;
      typename  QList<T>::iterator m_li;
   };

};


// ------------------------------------------------------------------------

// CopperSpice - we can add isCase to the constructor

/** A sorted dictionary of ClassDef objects. */
class ClassSDict : public StringMap<QSharedPointer<ClassDef>>
{
 public:
   ClassSDict(Qt::CaseSensitivity isCase = Qt::CaseSensitive)
      : StringMap<QSharedPointer<ClassDef>>(isCase)
   {}

   ~ClassSDict() {}

   void writeDeclaration(OutputList &ol, const enum CompoundType *filter = 0,
                         const QString &header = 0, bool localNames = false);

   void writeDocumentation(OutputList &ol, QSharedPointer<Definition> container = QSharedPointer<Definition>());
   bool declVisible(const enum CompoundType *filter = 0) const;

 private:
   int compareMapValues(const QSharedPointer<ClassDef> &item1, const QSharedPointer<ClassDef> &item2) const override;
};

/** A sorted dictionary of DirDef objects. */
class DirSDict : public StringMap<QSharedPointer<DirDef>>
{
   public:
      DirSDict() : StringMap<QSharedPointer<DirDef>>() {}

      int compareMapValues(const QSharedPointer<DirDef> &item1, const QSharedPointer<DirDef> &item2) const override;
};

/** A sorted dictionary of Example objects. */
class ExampleSDict : public StringMap<QSharedPointer<Example>>
{
   public:
      ExampleSDict() : StringMap<QSharedPointer<Example>>() {}
      ~ExampleSDict() {}

   private:
      int compareMapValues(const QSharedPointer<Example> &item1, const QSharedPointer<Example> &item2) const override;
};

/** map of FileNameList objects */
class FileNameStringCompare {
   public:
      bool operator()(const QString &a, const QString &b) const {
         static const Qt::CaseSensitivity sortCase_enum = Config::getCase("case-sensitive-fname");

         return a.compare(b, sortCase_enum) < 0;
      }
};

class FileNameDict : public StringMap<QSharedPointer<FileNameList>, FileNameStringCompare>
{
   public:
      FileNameDict() : StringMap<QSharedPointer<FileNameList>, FileNameStringCompare>(FileNameStringCompare())
      { }

      ~FileNameDict() {}
};

/** A sorted dictionary of FilePair objects. */
class FilePairDict : public StringMap<QSharedPointer<FilePair>>
{
   public:
      FilePairDict() : StringMap<QSharedPointer<FilePair>>() {}

   private:
      int compareMapValues(const QSharedPointer<FilePair> &item1, const QSharedPointer<FilePair> &item2) const override;
};

/** A sorted dictionary of GroupDef objects. */
class GroupSDict : public StringMap<QSharedPointer<GroupDef>>
{
   public:
      GroupSDict() : StringMap<QSharedPointer<GroupDef>>() {}
      virtual ~GroupSDict() {}

   private:
      int compareMapValues(const QSharedPointer<GroupDef> &item1, const QSharedPointer<GroupDef> &item2) const override;
};


/** Sorted dictionary of IndexField objects. */
class IndexFieldSDict : public StringMap<QSharedPointer<IndexField>>
{
   public:
      IndexFieldSDict() : StringMap<QSharedPointer<IndexField>>() {}
      ~IndexFieldSDict() {}

   private:
     int compareMapValues(const QSharedPointer<IndexField> &item1, const QSharedPointer<IndexField> &item2) const override;
};

/** A sorted dictionary of MemberDef objects. */
class MemberSDict : public StringMap<QSharedPointer<MemberDef>>
{
   public:
      MemberSDict() : StringMap<QSharedPointer<MemberDef>>() {}
      virtual ~MemberSDict() {}

   private:
      int compareMapValues(const QSharedPointer<MemberDef> &item1, const QSharedPointer<MemberDef> &item2) const override;
};

/** Sorted dictionary of MemberName objects */
class MemberNameSDict : public StringMap<QSharedPointer<MemberName>>
{
   public:
      MemberNameSDict() : StringMap<QSharedPointer<MemberName>>() {}
      ~MemberNameSDict() {}

   private:
      int compareMapValues(const QSharedPointer<MemberName> &item1, const QSharedPointer<MemberName> &item2) const override;
};

/** Sorted dictionary of MemberNameInfo objects. */
class MemberNameInfoSDict : public StringMap<QSharedPointer<MemberNameInfo>>
{
   public:
      MemberNameInfoSDict() : StringMap<QSharedPointer<MemberNameInfo>>() {}
      ~MemberNameInfoSDict() {}

   private:
      int compareMapValues(const QSharedPointer<MemberNameInfo> &item1, const QSharedPointer<MemberNameInfo> &item2)
             const override;
};

/** sorted dictionary of NamespaceDef objects. */
class NamespaceSDict : public StringMap<QSharedPointer<NamespaceDef>>
{
   public:
      // CopperSpice - can add isCase
      NamespaceSDict() : StringMap<QSharedPointer<NamespaceDef>>() {}
      ~NamespaceSDict() {}

      void writeDeclaration(OutputList &ol, const QString &title, bool isConstantGroup = false, bool localName = false);
      bool declVisible() const;

   private:
      int compareMapValues(const QSharedPointer<NamespaceDef> &item1, const QSharedPointer<NamespaceDef> &item2)
            const override;
};

class PageSDict : public StringMap<QSharedPointer<PageDef>>
{
   public:
      // CopperSpice - can add isCase
      PageSDict() : StringMap<QSharedPointer<PageDef>>() {}
      virtual ~PageSDict() {}

   private:
      int compareMapValues(const QSharedPointer<PageDef> &i1, const QSharedPointer<PageDef> &i2) const override;
};


class SearchIndexMap : public StringMap<QSharedPointer<SearchDefinitionList>>
{
 public:
   SearchIndexMap(uint letter) :
                  StringMap<QSharedPointer<SearchDefinitionList>>(Qt::CaseInsensitive), m_letter(letter)
   { }

   ~SearchIndexMap()
   { }

   void insertDef(QSharedPointer<Definition> d);
   uint letter() const;

 private:
   int compareMapValues(const QSharedPointer<SearchDefinitionList> &md1,
                  const QSharedPointer<SearchDefinitionList> &md2) const override;

   uint m_letter;
};


// ------------------------------------------------------------------------

/** A sorted dictionary of MemberGroup objects. */
class MemberGroupSDict : public LongMap<QSharedPointer<MemberGroup>>
{
 public:
   MemberGroupSDict() : LongMap<QSharedPointer<MemberGroup>>() {}
   ~MemberGroupSDict() {}

 private:
   int compareMapValues(const QSharedPointer<MemberGroup> &item1, const QSharedPointer<MemberGroup> &item2) const override;

};

using SectionDict = StringMap<QSharedPointer<SectionInfo>>;
using UseSDict    = StringMap<UseEntry *>;

#endif