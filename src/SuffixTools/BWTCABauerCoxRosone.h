//-----------------------------------------------
// Copyright 2011 Wellcome Trust Sanger Institute
// Written by Jared Simpson (js18@sanger.ac.uk)
// Released under the GPL license
//-----------------------------------------------
//
// BWTCABauerCoxRosone - In-memory version of Illumina's
// BWT construction algorithm
#ifndef BWTCA_COX_BAUER_ROSONE_H
#define BWTCA_COX_BAUER_ROSONE_H

#include "ReadTable.h"
#include "Alphabet.h"
#include "EncodedString.h"

namespace BWTCA
{
    // This structure tracks the state of each read
    // It contains a position field, which stores the relative/absolute
    // position to insert the next symbol of a read into the partial BWT.
    // It also stores the read index and the symbol to insert
    struct BCRElem
    {
        BCRElem() : index(0), sym('\0') {}

        // Comparator. Sort first on the symbol, then break ties using the index
        friend bool operator<(const BCRElem& a, const BCRElem& b)
        {
            return a.position < b.position;
        }

        friend std::ostream& operator<<(std::ostream& out, const BCRElem& elem)
        {
            out << "I: " << elem.index << " S: " << elem.sym << " P: " << elem.position;
            return out;
        }

        // Data
        uint64_t position; // the relative position into the next partial bwt to insert a symbol
        uint32_t index; // the read index this entry represents
        char sym; // a symbol to be inserted
    };
    typedef std::vector<BCRElem> BCRVector;

    // Construct the burrows-wheeler transform of the table of reads
    void runBauerCoxRosone(const ReadTable* pRT);
    
    // Run the initial special first iteration of the algorithm
    void outputInitialCycle(const ReadTable* pRT, BCRVector& bcrVector, BWTString& bwt, AlphaCount64& suffixSymbolCounts);

    size_t outputPartialCycle(int cycle,
                             const ReadTable* pRT, 
                             BCRVector& bcrVector, 
                             const BWTString& readBWT, 
                             size_t total_read_symbols,
                             BWTString& writeBWT, 
                             AlphaCount64& suffixStartCounts);

    // Calculate absolute position for each element of the nvector
    void calculateAbsolutePositions(BCRVector& bcrVector, const AlphaCount64& suffixSymbolCounts);
};

#endif
