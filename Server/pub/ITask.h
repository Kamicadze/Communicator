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
	ITask();
	virtual ~ITask();
	virtual void m_run()=0;
	virtual void m_showTask()=0;
};
