/*! \brief Small interface for queue datatype
 *
 * This interface will be usedas a data type for queue. Some methods will in
 * WorkQueue will use it as a data type too. All the tasks from each handlers
 * will be added to queue.
 *
 *
 */

#pragma once

#include <cstdio>

class ITask
{
public:
    virtual ~ITask(){};
    virtual void run()=0;
};

