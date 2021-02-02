#include "CharQ.h"

#include "debugLog.h"

extern DebugLog dbg;


bool CharQ::connect( const char* from, const char* to) {
  bool rc = false;
  CircularQBase<char>* f = find( from);
  if( f) {
    CircularQBase<char>* t = find( to);
    if( t) {
      f->setNextQ( t);
      rc = true;
    }
  }
  return rc;
}

CQitem* CQitem::s_first = NULL;

CQitem::CQitem(const char* name, CircularQBase<char>* q) {
  dbg.print( __FILE__, __LINE__, -1, "CQitem::CQitem()" );
  m_name = name;
  m_q = q;
  if( s_first == NULL) {
    s_first = this;
  } else {
    m_next = s_first;
    s_first = this;
  }
}
CQitem::~CQitem() {
  CQitem* c = s_first;
  CQitem* last = NULL;
  while( c != NULL) {
    if( c == this) {
      if( last == NULL) {
        s_first = c->m_next;
      } else {
        last->m_next = c->m_next;
      }
      break;
    } else {
      last = c;
      c = c->m_next;
    }
  }
}

CQitem* CQitem::find( CircularQBase<char>* q) {
  CQitem* c = s_first;
  while( c ) {
    if( c->m_q == q) {
      break;
    } else {
      c = c->m_next;
    }
  }
  return c;
}


CircularQBase<char>* CQitem::find( const char* n) {
  CQitem* c = s_first;
  CircularQBase<char>* rc = NULL;
  while( c ) {
    if( !strcmp( n, c->m_name)) {
      rc = c->m_q;
      break;
    } else {
      c = c->m_next;
    }
  }
  return rc;
}

void CQitem::dump( ) {
  CQitem* c = s_first;
  CQitem* n;
  dbg.printX( __FILE__, __LINE__, -1, (uint32_t) c, "dump start" );
  while( c ) {
    dbg.print( __FILE__, __LINE__, -1, c->m_q->sliceName(), c->m_name );
    if( c->m_q->getNextQ()) {
      n = CQitem::find(c->m_q->getNextQ() );
      if( n) {
        dbg.print( __FILE__, __LINE__, -1, c->m_name, n->m_name );
      }

    }
    c = c->m_next;
  }
  dbg.printX( __FILE__, __LINE__, -1, (uint32_t) c, "dump end" );
}
