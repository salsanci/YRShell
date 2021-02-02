#ifndef CharQ_h
#define CharQ_h

#include "CircularQ.h"


class CQitem {
protected:
  CQitem *m_next;
  CircularQBase<char>* m_q;
  static CQitem* s_first;
  const char* m_name;
public:
  CQitem( const char* name = "", CircularQBase<char>* q = NULL);
  ~CQitem();
  static CircularQBase<char>* find( const char* n);
  static CQitem* find( CircularQBase<char>* q);
  static void dump( void);
};

class CharQ {
public:
     static CircularQBase<char>* find( const char* n) { return CQitem::find( n); } 
     static bool connect( const char* from, const char* to);
};

class CQ1024 : public CircularQ<char, 1024> {
protected:
  CQitem m_cq;
public:
  CQ1024(const char* name = NULL):m_cq{name, this} {}
  virtual ~CQ1024() {}
  virtual const char* sliceName( void) { return "CQ1024"; } 
  virtual void slice( void) { CircularQ<char, 1024>::slice(); }
};
class CQ512 : public CircularQ<char, 512> {
protected:
  CQitem m_cq;
public:
  CQ512(const char* name = NULL):m_cq{name, this} {}
  virtual ~CQ512() {}
  virtual const char* sliceName( void) { return "CQ512"; } 
  virtual void slice( void) { CircularQ<char, 512>::slice(); }
};
class CQ256 : public CircularQ<char, 256> {
protected:
  CQitem m_cq;
public:
  CQ256(const char* name = NULL):m_cq{name, this} {}
  virtual ~CQ256() {}
  virtual const char* sliceName( void) { return "CQ256"; } 
  virtual void slice( void) { CircularQ<char, 256>::slice(); }
};
class CQ64 : public CircularQ<char, 64> {
protected:
  CQitem m_cq;
public:
  CQ64(const char* name = NULL):m_cq{name, this} {}
  virtual ~CQ64() {}
  virtual const char* sliceName( void) { return "CQ64"; } 
  virtual void slice( void) { CircularQ<char, 64>::slice(); }
};
class CQ32 : public CircularQ<char, 32> {
protected:
  CQitem m_cq;
public:
  CQ32(const char* name = NULL):m_cq{name, this} {}
  virtual ~CQ32() {}
  virtual const char* sliceName( void) { return "CQ32"; } 
  virtual void slice( void) { CircularQ<char, 32>::slice(); }
};


#endif