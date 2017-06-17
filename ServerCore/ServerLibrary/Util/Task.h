#pragma once
#include "stdafx.h"

#define TICK_INFINTY		0

class Work;
class TaskNode
{
	Work		*workObject_;
	int			freqSec_;
	int			durationSec_;

	tick_t		nextTick_;
public:
	TaskNode(Work *workObject, int freqSec, int durationSec);
	~TaskNode();

	void nextTick();
	bool expired();

	void tick();
};

//---------------------------------------------------------------------//
class Task
{
	std::list<TaskNode *> taskList_;
	Thread		*thread_;
	int			id_;

public:
	Task(int id);
	~Task();
	void add(TaskNode *taskNode);
	void remove(TaskNode *taskNode);

	void process();
	void run();
};

//---------------------------------------------------------------------//
class TaskManager : public Singleton <TaskManager>
{
	int                     threadCount_;
	std::vector<Task *>     taskPool_;

public:
	TaskManager();
	virtual ~TaskManager();

	void initialize(xml_t *config);

	void add(Work *workObject, int freqSec, int durationSec);
};