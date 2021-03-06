#pragma once

#include <map>
#include <set>
#include <vector>
#include "IProcessor.h"
#include "Utility.h"

class SummaryProcessor : public IProcessor
{
private:
	/**
	*	@brief target is x64 or not
	*/
	const bool isTarget64_;

	/**
	*	@brief gflag
	*/
	const ULONG32 ntGlobalFlag_;

	struct UstRecord {
		ULONG64 ustAddress;
		ULONG64 count;
		ULONG64 totalSize;
		ULONG64 maxSize;
		ULONG64 largestEntry;
		bool operator< (const SummaryProcessor::UstRecord& rhs) const
		{
			return totalSize < rhs.totalSize;
		}
	};

	/**
	*	@brief ustAddress to UstRecord map
	*/
	std::map<ULONG64, UstRecord> records_;

	/**
	*	@brief operator (disabled)
	*	@note to avoid C4512 warning
	*/
	SummaryProcessor& operator=(const SummaryProcessor&);

	/**
	*	@brief print set of UstRecord
	*/
	void PrintUstRecords(std::set<UstRecord>& records);

	/**
	*	@brief get caller module base address
	*/
	ULONG64 GetCallerModule(ULONG64 ustAddress, std::vector<ModuleInfo> &loadedModules);

	/**
	*	@brief test ust has matched frame
	*	@param key [in] prefix search key
	*/
	BOOL HasMatchedFrame(ULONG64 ustAddress, const char *key);

	/**
	*	@brief print stack trace
	*	@param ustAddress [in] address of entry in user mode stack trace database
	*/
	void PrintStackTrace(ULONG64 ustAddress);

public:
	/**
	*	@brief constructor
	*/
	SummaryProcessor();

	/**
	*	@copydoc IProcessor::StartHeap()
	*/
	void StartHeap(ULONG64 /*heapAddress*/) {}

	/**
	*	@copydoc IProcessor::Register()
	*/
	void Register(ULONG64 ustAddress,
		ULONG64 size, ULONG64 address,
		ULONG64 userSize, ULONG64 userAddress);

	/**
	*	@copydoc IProcessor::FinishHeap()
	*/
	void FinishHeap(ULONG64 /*heapAddress*/) {}

	/**
	*	@brief print summary of heap usage
	*/
	void Print();

	/**
	*	@brief print summary of matched heap usage
	*	@param key [in] prefix search key
	*/
	void Print(const char *key);
};
