/**
 * Copyright (C) 2013 Regents of the University of California.
 * @author: Jeff Thompson <jefft0@remap.ucla.edu>
 * See COPYING for copyright and distribution information.
 */

#include <cstdlib>
#include <sstream>
#include <iostream>
#include <time.h>
#include <unistd.h>
#include <ndn-cpp/face.hpp>
#include <ndn-cpp/security/identity/memory-identity-storage.hpp>
#include <ndn-cpp/security/identity/memory-private-key-storage.hpp>
#include <ndn-cpp/security/policy/no-verify-policy-manager.hpp>
#include <ndn-cpp/security/key-chain.hpp>

using namespace std;
using namespace ndn;
using namespace ndn::ptr_lib;

static uint8_t DEFAULT_PUBLIC_KEY_DER[] = {
0x30, 0x81, 0x9F, 0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x81,
0x8D, 0x00, 0x30, 0x81, 0x89, 0x02, 0x81, 0x81, 0x00, 0xE1, 0x7D, 0x30, 0xA7, 0xD8, 0x28, 0xAB, 0x1B, 0x84, 0x0B, 0x17,
0x54, 0x2D, 0xCA, 0xF6, 0x20, 0x7A, 0xFD, 0x22, 0x1E, 0x08, 0x6B, 0x2A, 0x60, 0xD1, 0x6C, 0xB7, 0xF5, 0x44, 0x48, 0xBA,
0x9F, 0x3F, 0x08, 0xBC, 0xD0, 0x99, 0xDB, 0x21, 0xDD, 0x16, 0x2A, 0x77, 0x9E, 0x61, 0xAA, 0x89, 0xEE, 0xE5, 0x54, 0xD3,
0xA4, 0x7D, 0xE2, 0x30, 0xBC, 0x7A, 0xC5, 0x90, 0xD5, 0x24, 0x06, 0x7C, 0x38, 0x98, 0xBB, 0xA6, 0xF5, 0xDC, 0x43, 0x60,
0xB8, 0x45, 0xED, 0xA4, 0x8C, 0xBD, 0x9C, 0xF1, 0x26, 0xA7, 0x23, 0x44, 0x5F, 0x0E, 0x19, 0x52, 0xD7, 0x32, 0x5A, 0x75,
0xFA, 0xF5, 0x56, 0x14, 0x4F, 0x9A, 0x98, 0xAF, 0x71, 0x86, 0xB0, 0x27, 0x86, 0x85, 0xB8, 0xE2, 0xC0, 0x8B, 0xEA, 0x87,
0x17, 0x1B, 0x4D, 0xEE, 0x58, 0x5C, 0x18, 0x28, 0x29, 0x5B, 0x53, 0x95, 0xEB, 0x4A, 0x17, 0x77, 0x9F, 0x02, 0x03, 0x01,
0x00, 01  
};

static uint8_t DEFAULT_PRIVATE_KEY_DER[] = {
0x30, 0x82, 0x02, 0x5d, 0x02, 0x01, 0x00, 0x02, 0x81, 0x81, 0x00, 0xe1, 0x7d, 0x30, 0xa7, 0xd8, 0x28, 0xab, 0x1b, 0x84,
0x0b, 0x17, 0x54, 0x2d, 0xca, 0xf6, 0x20, 0x7a, 0xfd, 0x22, 0x1e, 0x08, 0x6b, 0x2a, 0x60, 0xd1, 0x6c, 0xb7, 0xf5, 0x44,
0x48, 0xba, 0x9f, 0x3f, 0x08, 0xbc, 0xd0, 0x99, 0xdb, 0x21, 0xdd, 0x16, 0x2a, 0x77, 0x9e, 0x61, 0xaa, 0x89, 0xee, 0xe5,
0x54, 0xd3, 0xa4, 0x7d, 0xe2, 0x30, 0xbc, 0x7a, 0xc5, 0x90, 0xd5, 0x24, 0x06, 0x7c, 0x38, 0x98, 0xbb, 0xa6, 0xf5, 0xdc,
0x43, 0x60, 0xb8, 0x45, 0xed, 0xa4, 0x8c, 0xbd, 0x9c, 0xf1, 0x26, 0xa7, 0x23, 0x44, 0x5f, 0x0e, 0x19, 0x52, 0xd7, 0x32,
0x5a, 0x75, 0xfa, 0xf5, 0x56, 0x14, 0x4f, 0x9a, 0x98, 0xaf, 0x71, 0x86, 0xb0, 0x27, 0x86, 0x85, 0xb8, 0xe2, 0xc0, 0x8b,
0xea, 0x87, 0x17, 0x1b, 0x4d, 0xee, 0x58, 0x5c, 0x18, 0x28, 0x29, 0x5b, 0x53, 0x95, 0xeb, 0x4a, 0x17, 0x77, 0x9f, 0x02,
0x03, 0x01, 0x00, 0x01, 0x02, 0x81, 0x80, 0x1a, 0x4b, 0xfa, 0x4f, 0xa8, 0xc2, 0xdd, 0x69, 0xa1, 0x15, 0x96, 0x0b, 0xe8,
0x27, 0x42, 0x5a, 0xf9, 0x5c, 0xea, 0x0c, 0xac, 0x98, 0xaa, 0xe1, 0x8d, 0xaa, 0xeb, 0x2d, 0x3c, 0x60, 0x6a, 0xfb, 0x45,
0x63, 0xa4, 0x79, 0x83, 0x67, 0xed, 0xe4, 0x15, 0xc0, 0xb0, 0x20, 0x95, 0x6d, 0x49, 0x16, 0xc6, 0x42, 0x05, 0x48, 0xaa,
0xb1, 0xa5, 0x53, 0x65, 0xd2, 0x02, 0x99, 0x08, 0xd1, 0x84, 0xcc, 0xf0, 0xcd, 0xea, 0x61, 0xc9, 0x39, 0x02, 0x3f, 0x87,
0x4a, 0xe5, 0xc4, 0xd2, 0x07, 0x02, 0xe1, 0x9f, 0xa0, 0x06, 0xc2, 0xcc, 0x02, 0xe7, 0xaa, 0x6c, 0x99, 0x8a, 0xf8, 0x49,
0x00, 0xf1, 0xa2, 0x8c, 0x0c, 0x8a, 0xb9, 0x4f, 0x6d, 0x73, 0x3b, 0x2c, 0xb7, 0x9f, 0x8a, 0xa6, 0x7f, 0x9b, 0x9f, 0xb7,
0xa1, 0xcc, 0x74, 0x2e, 0x8f, 0xb8, 0xb0, 0x26, 0x89, 0xd2, 0xe5, 0x66, 0xe8, 0x8e, 0xa1, 0x02, 0x41, 0x00, 0xfc, 0xe7,
0x52, 0xbc, 0x4e, 0x95, 0xb6, 0x1a, 0xb4, 0x62, 0xcc, 0xd8, 0x06, 0xe1, 0xdc, 0x7a, 0xa2, 0xb6, 0x71, 0x01, 0xaa, 0x27,
0xfc, 0x99, 0xe5, 0xf2, 0x54, 0xbb, 0xb2, 0x85, 0xe1, 0x96, 0x54, 0x2d, 0xcb, 0xba, 0x86, 0xfa, 0x80, 0xdf, 0xcf, 0x39,
0xe6, 0x74, 0xcb, 0x22, 0xce, 0x70, 0xaa, 0x10, 0x00, 0x73, 0x1d, 0x45, 0x0a, 0x39, 0x51, 0x84, 0xf5, 0x15, 0x8f, 0x37,
0x76, 0x91, 0x02, 0x41, 0x00, 0xe4, 0x3f, 0xf0, 0xf4, 0xde, 0x79, 0x77, 0x48, 0x9b, 0x9c, 0x28, 0x45, 0x26, 0x57, 0x3c,
0x71, 0x40, 0x28, 0x6a, 0xa1, 0xfe, 0xc3, 0xe5, 0x37, 0xa1, 0x03, 0xf6, 0x2d, 0xbe, 0x80, 0x64, 0x72, 0x69, 0x2e, 0x9b,
0x4d, 0xe3, 0x2e, 0x1b, 0xfe, 0xe7, 0xf9, 0x77, 0x8c, 0x18, 0x53, 0x9f, 0xe2, 0xfe, 0x00, 0xbb, 0x49, 0x20, 0x47, 0xdf,
0x01, 0x61, 0x87, 0xd6, 0xe3, 0x44, 0xb5, 0x03, 0x2f, 0x02, 0x40, 0x54, 0xec, 0x7c, 0xbc, 0xdd, 0x0a, 0xaa, 0xde, 0xe6,
0xc9, 0xf2, 0x8d, 0x6c, 0x2a, 0x35, 0xf6, 0x3c, 0x63, 0x55, 0x29, 0x40, 0xf1, 0x32, 0x82, 0x9f, 0x53, 0xb3, 0x9e, 0x5f,
0xc1, 0x53, 0x52, 0x3e, 0xac, 0x2e, 0x28, 0x51, 0xa1, 0x16, 0xdb, 0x90, 0xe3, 0x99, 0x7e, 0x88, 0xa4, 0x04, 0x7c, 0x92,
0xae, 0xd2, 0xe7, 0xd4, 0xe1, 0x55, 0x20, 0x90, 0x3e, 0x3c, 0x6a, 0x63, 0xf0, 0x34, 0xf1, 0x02, 0x41, 0x00, 0x84, 0x5a,
0x17, 0x6c, 0xc6, 0x3c, 0x84, 0xd0, 0x93, 0x7a, 0xff, 0x56, 0xe9, 0x9e, 0x98, 0x2b, 0xcb, 0x5a, 0x24, 0x4a, 0xff, 0x21,
0xb4, 0x9e, 0x87, 0x3d, 0x76, 0xd8, 0x9b, 0xa8, 0x73, 0x96, 0x6c, 0x2b, 0x5c, 0x5e, 0xd3, 0xa6, 0xff, 0x10, 0xd6, 0x8e,
0xaf, 0xa5, 0x8a, 0xcd, 0xa2, 0xde, 0xcb, 0x0e, 0xbd, 0x8a, 0xef, 0xae, 0xfd, 0x3f, 0x1d, 0xc0, 0xd8, 0xf8, 0x3b, 0xf5,
0x02, 0x7d, 0x02, 0x41, 0x00, 0x8b, 0x26, 0xd3, 0x2c, 0x7d, 0x28, 0x38, 0x92, 0xf1, 0xbf, 0x15, 0x16, 0x39, 0x50, 0xc8,
0x6d, 0x32, 0xec, 0x28, 0xf2, 0x8b, 0xd8, 0x70, 0xc5, 0xed, 0xe1, 0x7b, 0xff, 0x2d, 0x66, 0x8c, 0x86, 0x77, 0x43, 0xeb,
0xb6, 0xf6, 0x50, 0x66, 0xb0, 0x40, 0x24, 0x6a, 0xaf, 0x98, 0x21, 0x45, 0x30, 0x01, 0x59, 0xd0, 0xc3, 0xfc, 0x7b, 0xae,
0x30, 0x18, 0xeb, 0x90, 0xfb, 0x17, 0xd3, 0xce, 0xb5
};

class Echo {
public:
  Echo(KeyChain &keyChain, const Name& certificateName)
  : keyChain_(keyChain), certificateName_(certificateName), responseCount_(0)
  { 
  }
  
  // onInterest.
  void operator()
     (const shared_ptr<const Name>& prefix, const shared_ptr<const Interest>& interest, Transport& transport,
      uint64_t registeredPrefixId) 
  {
    ++responseCount_;
    
    // Make and sign a Data packet.
    Data data(interest->getName());
    string content(string("Echo ") + interest->getName().toUri());
    data.setContent((const uint8_t *)&content[0], content.size());
    data.getMetaInfo().setTimestampMilliseconds(time(NULL) * 1000.0);
    keyChain_.sign(data, certificateName_);
    Blob encodedData = data.wireEncode();

    cout << "Sent content " << content << endl;
    transport.send(*encodedData);
  }
  
  // onRegisterFailed.
  void operator()(const shared_ptr<const Name>& prefix)
  {
    ++responseCount_;
    cout << "Register failed for prefix " << prefix->toUri() << endl;
  }

  KeyChain keyChain_;
  Name certificateName_;
  int responseCount_;
};

int main(int argc, char** argv)
{
  try {
    Face face("localhost");
        
    shared_ptr<MemoryIdentityStorage> identityStorage(new MemoryIdentityStorage());
    shared_ptr<MemoryPrivateKeyStorage> privateKeyStorage(new MemoryPrivateKeyStorage());
    KeyChain keyChain
      (make_shared<IdentityManager>(identityStorage, privateKeyStorage), make_shared<NoVerifyPolicyManager>());
    keyChain.setFace(&face);
    
    // Initialize the storage.
    Name keyName("/testname/DSK-123");
    Name certificateName = keyName.getSubName(0, keyName.size() - 1).append("KEY").append
           (keyName.get(keyName.size() - 1)).append("ID-CERT").append("0");
    identityStorage->addKey(keyName, KEY_TYPE_RSA, Blob(DEFAULT_PUBLIC_KEY_DER, sizeof(DEFAULT_PUBLIC_KEY_DER)));
    privateKeyStorage->setKeyPairForKeyName
      (keyName, DEFAULT_PUBLIC_KEY_DER, sizeof(DEFAULT_PUBLIC_KEY_DER), DEFAULT_PRIVATE_KEY_DER, sizeof(DEFAULT_PRIVATE_KEY_DER));
   
    Echo echo(keyChain, certificateName);
    Name prefix("/testecho");
    cout << "Register prefix  " << prefix.toUri() << endl;
    face.registerPrefix(prefix, ref(echo), ref(echo));
    
    // The main event loop.  
    // Wait forever to receive one interest for the prefix.
    while (echo.responseCount_ < 1) {
      face.processEvents();
      // We need to sleep for a few milliseconds so we don't use 100% of the CPU.
      usleep(10000);
    }
  } catch (std::exception& e) {
    cout << "exception: " << e.what() << endl;
  }
  return 0;
}
