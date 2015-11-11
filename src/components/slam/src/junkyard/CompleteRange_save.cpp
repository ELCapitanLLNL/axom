/*
 * Copyright (c) 2015, Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory.
 *
 * All rights reserved.
 *
 * This source code cannot be distributed without permission and further
 * review from Lawrence Livermore National Laboratory.
 */


/*
 * OrderedSet.cpp
 *
 *  Created on: Apr 23, 2015
 *      Author: weiss27
 */

#include <stdexcept>
#include <sstream>
#include <iostream>
#include "RangeSet.hpp"

namespace asctoolkit {
namespace slam {


const NullSet RangeSet::s_nullSet;

bool RangeSet::isValid(bool verboseOutput) const
{
    bool bValid = true;

    std::stringstream errStr;

//    if( m_size < 0 )
//    {
//        if(verboseOutput)
//        {
//            errStr <<"\n** OrderedSet's size cannot be less than zero"
//                    <<"\n\t Size was " << m_size
//                    ;
//        }
//        bValid = false;
//    }


    if(verboseOutput)
    {
        std::stringstream sstr;

        sstr<<"\n*** Detailed results of isValid on the OrderedSet.\n";
        if(bValid)
        {
            sstr<<"Set was valid."<< std::endl;
        }
        else
        {
            sstr<<"Set was NOT valid.\n"
                     << errStr.str()
                     << std::endl;
        }

        sstr<<"\n** size is " << m_size ;
        std::cout<< sstr.str() << std::endl;
    }

    return bValid;
}

} /* namespace slam */
} /* namespace asctoolkit */

