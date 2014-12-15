/*************************************************************************
 *
 * Copyright (C) 1997-2014 by Dimitri van Heesch.
 * Copyright (C) 2014-2015 Barbara Geller & Ansel Sermersheim 
 * All rights reserved.    
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation under the terms of the GNU General Public License version 2
 * is hereby granted. No representations are made about the suitability of
 * this software for any purpose. It is provided "as is" without express or
 * implied warranty. See the GNU General Public License for more details.
 *
 * Documents produced by Doxygen are derivative works derived from the
 * input used in their production; they are not affected by this license.
 *
*************************************************************************/

#ifndef MAP_H
#define MAP_H

#include <QList>
#include <QHash>

#include <typeinfo>
#include <algorithm>
#include <stdexcept>

/** Ordered dictionary of elements of type T.
 *  Internally uses a QMap<T>.
 */
template<class T>
class StringMap
{
 private:  
   QMap<QString, T> m_dict;
   
 public:
   /*! Create an ordered dictionary.   
    *  \param caseSensitive indicated whether the keys should be sorted
    *         in a case sensitive way.
    */
   StringMap(Qt::CaseSensitivity foo = Qt::CaseSensitive) {

      if (foo) {
         // CS BROOM - add case stuff
         // m_dict = QMap<QString, T>;
      } else { 
         // m_dict = QMap<QString, T>;
      }    
   }

   /*! Destroys the dictionary */
   virtual ~StringMap() {            
   }

   using iterator = typename QMap<QString, T>::iterator;
   
   iterator begin() {     
      return m_dict.begin();      
   }  

   iterator end() {     
      return m_dict.end();  
   }  

   using const_iterator = typename QMap<QString, T>::const_iterator;
   
   const_iterator begin() const {     
      return m_dict.begin();      
   }  

   const_iterator end() const {     
      return m_dict.end();  
   }  

   /*! Appends an element to the dictionary. The element is owned by the
    *  dictionary.
    *  \param key The unique key to use to quicky find the item later on.
    *  \param d The compound to add.
    *  \sa find()
    */
   void insert(const char *key, const T &d) {     
      m_dict.insert(key, d);   
   }  

   void insert(QByteArray key, const T &d) {     
      m_dict.insert(key, d);   
   }  

   void insert(QString key, const T &d) {     
      m_dict.insert(key, d);   
   }  

   /*! Remove an item from the dictionary */
   bool remove(const char *key) { 
      return m_dict.remove(key); 
   }

   /*! Take an item out of the dictionary without deleting it */
   T *take(const char *key) {
      return m_dict.take(key);
   } 
   
   /*! Looks up a compound given its key.
    *  \param key The key to identify this element.
    *  \return The requested compound or zero if it cannot be found.
    *  \sa append()
    */
   T find(const char *key) {
       auto item = m_dict.find(key);

      if (item == m_dict.end()) {   
         return T();
      }

      return item.value();
   }

   T find(const QByteArray &key) {
      auto item = m_dict.find(key);

      if (item == m_dict.end()) {   
         return T();
      }

      return item.value();
   }

   T find(const QString &key) {
       auto item = m_dict.find(key);

      if (item == m_dict.end()) {   
         return T();
      }

      return item.value();
   }

   /*! Equavalent to find(). */
   T *operator[](const char *key) const {
      return m_dict.find(key);
   }

   /*! Clears the dictionary.  
    */
   void clear() {
      m_dict->clear();
   }

   /*! Returns the number of items stored in the dictionary
    */
   int count() const {
      return m_dict.count();
   }

   int compareValues(const T &item1, const T &item2) const {

      if (item1 < item2) {
         return -1;
      }

      return 0;
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

         QList<T> temp1 = dict.m_dict.values();

         QList<T> temp2 = temp1;
         std::sort(temp2.begin(), temp2.end(), [&dict](const T &v1, const T &v2){ return dict.compareValues(v1, v2) < 0; } );

         if (temp1 != temp2) {
            std::string msg = "StringMap::Iterator Key and Value are not the same data "; 
            msg += typeid(T).name();

            throw std::runtime_error(msg);
         }   

         // save m_list
         m_list = temp2;
         m_li   = m_list.begin();   
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
      T toLast()  {
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
   LongMap() {
   }

   /*! Destroys the dictionary */
   virtual ~LongMap() {            
   }

   using iterator = typename QMap<long, T>::iterator;
   
   iterator begin() {     
      return m_dict.begin();      
   }  

   iterator end() {     
      return m_dict.end();  
   }  

   /*! Appends an element to the dictionary. The element is owned by the
    *  dictionary.
    *  \param key The unique key to use to quicky find the item later on.
    *  \param d The compound to add.
    *  \sa find()
    */
   void insert(long key, const T &d) {     
      m_dict.insert(key, d);   
   }  

   /*! Remove an item from the dictionary */
   bool remove(long key) { 
      return m_dict.remove(key); 
   }

   /*! Take an item out of the dictionary without deleting it */
   T *take(long key) {
      return m_dict.take(key);
   } 
   
   /*! Looks up a compound given its key.
    *  \param key The key to identify this element.
    *  \return The requested compound or zero if it cannot be found.
    *  \sa append()
    */
   T *find(long key) {
       auto item = m_dict.find(key);

      if (item == m_dict.end()) {   
         return T();
      }

      return item.value();
   }

   /*! Equavalent to find(). */
   T *operator[](long key) const {
      return m_dict.find(key);
   }

   /*! Clears the dictionary.  
    */
   void clear() {
      m_dict.clear();
   }

   /*! Returns the number of items stored in the dictionary
    */
   int count() const {
      return m_dict.count();
   }

   int compareValues(const T &item1, const T &item2) const {

      if (item1 < item2) {
         return -1;
      }

      return 0;
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

         QList<T> temp1 = dict.m_dict.values();

         QList<T> temp2 = temp1;
         std::sort(temp2.begin(), temp2.end(), [&dict](const T &v1, const T &v2){ return dict.compareValues(v1, v2) < 0; } );

         if (temp1 != temp2) {
            std::string msg = "LongMap::Iterator Key and Value are not the same data "; 
            msg += typeid(T).name();

            throw std::runtime_error(msg);
         }        

         // save m_list
         m_list = temp2;
         m_li   = m_list.begin();
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

#endif