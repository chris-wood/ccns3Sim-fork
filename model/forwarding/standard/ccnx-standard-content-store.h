/*
 * Copyright (c) 2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL XEROX OR PARC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/* ################################################################################
 * #
 * # PATENT NOTICE
 * #
 * # This software is distributed under the BSD 2-clause License (see LICENSE
 * # file).  This BSD License does not make any patent claims and as such, does
 * # not act as a patent grant.  The purpose of this section is for each contributor
 * # to define their intentions with respect to intellectual property.
 * #
 * # Each contributor to this source code is encouraged to state their patent
 * # claims and licensing mechanisms for any contributions made. At the end of
 * # this section contributors may each make their own statements.  Contributor's
 * # claims and grants only apply to the pieces (source code, programs, text,
 * # media, etc) that they have contributed directly to this software.
 * #
 * # There is no guarantee that this section is complete, up to date or accurate. It
 * # is up to the contributors to maintain their section in this file up to date
 * # and up to the user of the software to verify any claims herein.
 * #
 * # Do not remove this header notification.  The contents of this section must be
 * # present in all distributions of the software.  You may only modify your own
 * # intellectual property statements.  Please provide contact information.
 *
 * - Palo Alto Research Center, Inc
 * This software distribution does not grant any rights to patents owned by Palo
 * Alto Research Center, Inc (PARC). Rights to these patents are available via
 * various mechanisms. As of January 2016 PARC has committed to FRAND licensing any
 * intellectual property used by its contributions to this software. You may
 * contact PARC at cipo@parc.com for more information or visit http://www.ccnx.org
 */

#ifndef CCNS3SIM_MODEL_FORWARDING_STANDARD_CCNX_STANDARD_CONTENT_STORE_H_
#define CCNS3SIM_MODEL_FORWARDING_STANDARD_CCNX_STANDARD_CONTENT_STORE_H_

#include <map>
#include "ns3/ccnx-content-store.h"
#include "ns3/ccnx-standard-content-store-entry.h"
#include "ns3/ccnx-standard-forwarder-work-item.h"
#include "ns3/ccnx-delay-queue.h"
#include "ns3/ccnx-name.h"
#include "ns3/ccnx-hash-value.h"
#include "ns3/log.h"

namespace ns3 {
namespace ccnx {

#define KEYIDHACK
class CCNxStandardContentStore : public CCNxContentStore
{
public:
  /**
   * Returns the NS3 runtime type of the class ("ns3::ccnx::CCNxStandardContentStore")
   * @return The NS3 runtime type
   */
  static TypeId GetTypeId ();

  /**
   * Create a standard content store.  It is configured via the NS attribute system.
   */
  CCNxStandardContentStore ();

  /**
   * Virtual destructor for inheritance
   */
  virtual ~CCNxStandardContentStore ();


  /**
   * @copydoc CCNxContentStore::MatchInterest()
   */
  virtual void MatchInterest (Ptr<CCNxForwarderMessage> message);

  /**
   * @copydoc CCNxContentStore::SetMatchInterestCallback()
   */
  virtual void SetMatchInterestCallback (MatchInterestCallback matchInterestCallback);

  /**
   * @copydoc CCNxContentStore::AddContentObject()
   */
  virtual void AddContentObject (Ptr<CCNxForwarderMessage> message, Ptr<CCNxConnectionList> egressConnections);

  /**
   * @copydoc CCNxContentStore::AddContentObjectCallback()
   */
  virtual void SetAddContentObjectCallback (AddContentObjectCallback addContentObjectCallback);


//TODO make ServiceMatchInterest and other internal methods private:

  /**
   * After input delay, service a MatchInterest request and send the
   * result to the MatchInterestCallback.
   *
   * @param workItem [in] The work Item containing the interest being serviced
   */
  void ServiceMatchInterest (Ptr<CCNxStandardForwarderWorkItem> workItem);

  /**
   * After input delay, service a AddContentObject request and send the
   * result to the AddContentObjectCallback.
   *
   * @param workItem [in] The work Item containing the interest being serviced
   * Adds a content object to the content store.
   *
   * The caller should ensure that only the desired per-hop headers are in the packet.
   *
   * @param object The content object packet to add
   * @return true If the object was added
   * @return false If the object was not added
   */
  virtual void ServiceAddContentObject (Ptr<CCNxStandardForwarderWorkItem> workItem);

  /**
   * Removes the given object from the content store
   *
   * This should be the same object (name, keyid, hash) as was added previously, or
   * found via `MatchInterest()`.
   *
   * @param object The content object packet to remove
   * @return true if removed
   * @return false If not removed (i.e. not found)
   */
  virtual bool RemoveContentObject (Ptr<CCNxPacket> cPacket);

  /**
   * Returns the number of content object packets in the content store.
   *
   * The size of the store is usually configured via a SetAttribute.
   *
   * @return The number of objects in the store.
   */
  virtual size_t GetObjectCount () const;

  /**
   * Returns the maximum capacity (in content objects) of the content store.
   *
   * The size of the store is usually configured via a SetAttribute.
   *
   * @return The object capacity of the content store
   */
  virtual size_t GetObjectCapacity () const;

  /**
   * return entry pointer if this object in the content store, null otherwise.
   *
   */

  Ptr<CCNxStandardContentStoreEntry> FindEntryInHashMap(Ptr<CCNxPacket> cPacket);


  /**
   * Returns the number of content object in the hash map.
   *
   * @return The number of objects in the hash map.
   */
  virtual size_t GetMapByHashCount() const;

  /**
   * Returns the number of content object in the name map.
   *
   * @return The number of objects in the name map.
   */
  virtual size_t GetMapByNameCount() const;

  /**
   * Returns the number of content object in the nameKeyid map.
   *
   * @return The number of objects in the nameKeyid map.
   */
  virtual size_t GetMapByNameKeyidCount() const;

  virtual bool IsEntryValid(Ptr<CCNxStandardContentStoreEntry> entry) const;

  virtual Ptr<CCNxPacket> GetLruListTailPacket () const;

  virtual bool AddLruListEntry(Ptr<CCNxStandardContentStoreEntry> entry);

  virtual bool DeleteLruListEntry(Ptr<CCNxStandardContentStoreEntry> entry);

  virtual bool RefreshLruListEntry(Ptr<CCNxStandardContentStoreEntry> entry);

  virtual bool DeleteLruListPacket(Ptr<CCNxPacket> cPacket);

  virtual bool AddMapEntry(Ptr<CCNxPacket> cPacket, Ptr<CCNxStandardContentStoreEntry> newEntry);

  /**
    * Function to compare two smart pointers to CCNx packets based on Name and Keyid.
    * Uses the KeyId on a validation of a content object, or KeyidRestriction on an interest.
    * used in std::map as the comparison function to properly order content store map entries.
    *
    * Example:
    * @code
    * {
    *   std::map<Ptr<CCNxPacket>,Ptr<CCNxStandardContentStoreEntry>,isLessPtrCCNxNameKeyid> m_csByNameKeyid;
    * }
    * @endcode
    */
   struct isLessPtrCCNxNameKeyid
    {
      //mutable allows assignment inside function which must be marked const
      mutable Ptr<CCNxInterest> zInterest;
      mutable Ptr<CCNxHashValue> aKeyid,bKeyid,zKeyid;

      Ptr<CCNxHashValue> GetKeyIdFromMessage(Ptr<const CCNxPacket> z) const
        {
        switch (z->GetMessage()->GetMessageType())
        {
        case CCNxMessage::Interest :
	  zInterest = DynamicCast<CCNxInterest, CCNxMessage>(z->GetMessage());
	  zKeyid = zInterest->GetKeyidRestriction();
	  break;
        case CCNxMessage::ContentObject :
    #ifdef KEYIDHACK
	  //hack  magic number keyIdRest into content
	  zKeyid = Create <CCNxHashValue> (55);    //TODO - remove when marc's keyid available
    #else
	  zKeyid = ???
    #endif
	      break;
        default:
            NS_ASSERT("cant find keyid - packet has bad message type");
            zKeyid = Ptr<CCNxHashValue> (0);
            break;
        }
        return zKeyid;

        }

      bool operator() (Ptr<const CCNxPacket> a, Ptr<const CCNxPacket> b) const
      {
        aKeyid = GetKeyIdFromMessage(a);
        bKeyid = GetKeyIdFromMessage(b);
        if (*aKeyid->GetValue() == *bKeyid->GetValue())
          {
            return (*a->GetMessage()->GetName() < *b->GetMessage()->GetName());
          }
        else
          {
            return (*aKeyid->GetValue() < *bKeyid->GetValue());
          }

      }
    };

    /**
       * Function to compare two smart pointers to CCNx packets based on HashValue
       * Uses the Computed Hash on a content object, or HashRestriction on an interest.
       * used in std::map as the comparison function to properly order content store map entries.
       *
       * Example:
       * @code
       * {
       *   std::map<Ptr<CCNxPacket>,Ptr<CCNxStandardContentStoreEntry>,isLessPtrCCNxHashValue> m_csByHashValue;
       * }
       * @endcode
       */
      struct isLessPtrCCNxHashValue
    {
      mutable Ptr<CCNxInterest> zInterest;

      Ptr<const CCNxByteArray> GetHashFromMessage(Ptr<const CCNxPacket> z) const
        {
        switch (z->GetMessage()->GetMessageType())
        {
        case CCNxMessage::Interest :
	  zInterest = DynamicCast<CCNxInterest, CCNxMessage>(z->GetMessage());
//          NS_LOG_DEBUG("interest packet:name="<< *z->GetMessage()->GetName() << ", hash value=" << zInterest->GetHashRestriction().GetValue);
	  return(zInterest->GetHashRestriction()->GetValue());
	  break;
        case CCNxMessage::ContentObject :
//          NS_LOG_DEBUG("content packet:name="<< *z->GetMessage()->GetName() << ", hash value=" << z->GetContentObjectHash().GetValue);
	  return(z->GetContentObjectHash().GetValue());
	  break;
        default:
            NS_ASSERT("cant find hash - packet has bad message type");
            return(Ptr<CCNxByteArray> (0));
            break;
        } //switch
        }

      bool operator() (Ptr<const CCNxPacket> a, Ptr<const CCNxPacket> b) const
      {
        return (*GetHashFromMessage(a) < *GetHashFromMessage(b));

      }
    };

      /**
         * Function to compare two smart pointers to CCNx packets based on CCNxName
         * used in std::map as the comparison function to properly order content store map entries.
         *
         * Example:
         * @code
         * {
         *   std::map<Ptr<CCNxPacket>,Ptr<CCNxStandardContentStoreEntry>,isLessPtrCCNxName> m_csByName;
         * }
         * @endcode
         */
        struct isLessPtrCCNxName
      {
        bool operator() (Ptr<const CCNxPacket> a, Ptr<const CCNxPacket> b) const
        {

          return (*a->GetMessage()->GetName() < *b->GetMessage()->GetName());
        }
      };


private:

  // ns3::Object::DoInitialize()
  virtual void DoInitialize ();

  /** Maxium number of content objects to store in content store */
  uint64_t m_objectCapacity;

  /**
    * The maps below hold pointers to content packets in the content store.
    * When a packet is added to the store, it is added to one or more maps.
    * It is always added to the hash list (since all content objects have a hash value),
    * but only added to the name list if it has a name,
    * and only added to the name/keyid list if it has a name and a keyid.
    *
    */

  //Names to CS entry lookup
  typedef std::map<Ptr<const CCNxPacket>,Ptr<CCNxStandardContentStoreEntry>,isLessPtrCCNxName>  CSByNameType;

  //Hash values to CS entry lookup
  typedef std::map<Ptr<const CCNxPacket>,Ptr<CCNxStandardContentStoreEntry>,isLessPtrCCNxHashValue> CSByHashType;

  //Name and Keyid to CS entry lookup
  typedef std::map<Ptr<const CCNxPacket>,Ptr<CCNxStandardContentStoreEntry>,isLessPtrCCNxNameKeyid> CSByNameKeyidType;
  //TODO ask marc can content object  have keyid but no name?

  CSByNameType m_csByName;
  CSByHashType m_csByHash;
  CSByNameKeyidType m_csByNameKeyid;


   /**
    * The Least Recently Used (lru) list has the least recently used packet at the end of the list,
    * and the most recently used at the beginning. The list allows the LRU algorithm to be used when
    * to evict packets when the content store has reached maximum size.
    */
   std::list<Ptr<CCNxStandardContentStoreEntry> > m_lruList;

   /**
    * Set by the forwarder as the callback for matchInterest
    */

  MatchInterestCallback m_matchInterestCallback;

  /**
     * Set by the forwarder as the callback for addContentObject
     */

  AddContentObjectCallback m_addContentObjectCallback;







//   /**
//    * Callback from delay queue after a work item has waited its service time
//    *
//    * @param item [in] The work item to service
//    */
//
//   void DequeueCallback (Ptr<CCNxForwarderMessage> message);

   /**
    * The storage type of the CCNxDelayQueue
    */
   typedef CCNxDelayQueue <CCNxStandardForwarderWorkItem> DelayQueueType;

   /**
    * Input queue used to simulate processing delay
    */
   Ptr<DelayQueueType> m_inputQueue;

   /**
    * Callback from delay queue to compute the service time of a work item
    *
    * @param item [in] The work item being serviced
    * @return The service time of the work item
    */
   Time GetServiceTime (Ptr<CCNxStandardForwarderWorkItem> workItem);

   /**
    * Callback from delay queue after a work item has waited its service time
    *
    * @param item [in] The work item to service
    */

   void ServiceInputQueue (Ptr<CCNxStandardForwarderWorkItem> workItem);

  /**
   * The layer delay is:
   *
   * \f$time = m\_layerDelayConstant + m\_layerDelaySlope * nameBytes\f$
   *
   * It is set via the attribute "LayerDelayConstant".  The default is 1 usec.
   */
  Time m_layerDelayConstant;

  /**
   * The layer delay is:
   *
   * \f$time = m\_layerDelayConstant + m\_layerDelaySlope * nameBytes\f$
   *
   * It is set via the attribute "LayerDelaySlope".  The default is 0.
   */
  Time m_layerDelaySlope;

  /**
   * The number of parallel servers processing the input delay queue.
   * The more servers there are, the lower the amortized delay is.
   *
   * This value is set via the attribute "LayerDelayServers".  The default is 1.
   */
  unsigned m_layerDelayServers;
};

}   /* namespace ccnx */
} /* namespace ns3 */

#endif /* CCNS3SIM_MODEL_FORWARDING_STANDARD_CCNX_STANDARD_CONTENT_STORE_H_ */