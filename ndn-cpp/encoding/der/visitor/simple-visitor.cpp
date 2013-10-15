/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/**
 * Copyright (C) 2013 Regents of the University of California.
 * @author: Yingdi Yu <yingdi@cs.ucla.edu>
 * @author: Jeff Thompson <jefft0@remap.ucla.edu>
 * See COPYING for copyright and distribution information.
 */

#include "simple-visitor.hpp"
#include "../der.hpp"
#include <ndn-cpp/encoding/oid.hpp>

using namespace std;

namespace ndn {

namespace der
{
  
Any 
SimpleVisitor::visit(DerBool& derBool)
{
  bool result = true;
  
  if(0 == derBool.getPayload()[0])
    result = false;
  
  return Any(result);
}

Any 
SimpleVisitor::visit(DerInteger& derInteger)
{
  return Any(derInteger.getPayload());
}

Any 
SimpleVisitor::visit(DerPrintableString& derPStr)
{
  return Any(string((const char*)&derPStr.getPayload()[0], derPStr.getPayload().size()));
}

Any 
SimpleVisitor::visit(DerBitString& derBStr)
{
  return Any(derBStr.getPayload());
}

Any 
SimpleVisitor::visit(DerNull& derNull)
{
  return Any();
}

Any 
SimpleVisitor::visit(DerOctetString& derOStr)
{
  vector<uint8_t> result(derOStr.getPayload());
  return Any(result);
}

Any 
SimpleVisitor::visit(DerOid& derOid)
{
  vector<int> intList;
  int offset = 0;

  vector<uint8_t>& blob = derOid.getPayload();
    
  int first = blob[offset];
  
  intList.push_back(first / 40);
  intList.push_back(first % 40);

  offset++;
  
  while(offset < blob.size()){
    intList.push_back(derOid.decode128(offset));
  }
  
  return Any(OID(intList));
}

Any 
SimpleVisitor::visit(DerSequence& derSeq)
{
  return Any();
}

#if 0 // TODO: Implmenent alternative to boost::posix_time::from_iso_string.
Any 
SimpleVisitor::visit(DerGtime& derGtime)
{
  string str((const char*)&derGtime.getPayload()[0], derGtime.getPayload().size());
  return Any(boost::posix_time::from_iso_string(str.substr(0, 8) + "T" + str.substr(8, 6)));
}
#endif

} // der

}
