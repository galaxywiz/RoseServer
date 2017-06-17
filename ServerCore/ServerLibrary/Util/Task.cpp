#include "stdafx.h"
#include "Task.h"

TaskNode::TaskNode(Work *workObject, int freqSec, int durationSec)
{
	workObject_ = workObject;
	freqSec_ = freqSec;
	durationSec_ = durationSec;
	this->nextTick();
}

TaskNode::~TaskNode()
{
	SAFE_DELETE(workObject_);
}

void TaskNode::nextTick()
{
	nextTick_ = NOW_TICK() + (tick_t)freqSec_;
}

bool TaskNode::expired()
{
	if (workObject_ == nullptr) {
		return true;
	}
	if (durationSec_ != TICK_INFINTY) {
		if (durationSec_ < NOW_TICK()) {
			return true;
		}
	}
	return false;
}

void TaskNode::tick()
{
	if (nextTick_ < NOW_TICK()) {
		workObject_->tick();
		this->nextTick();
	}
}

//---------------------------------------------------------------------//
Task::Task(int id) 
{
	id_ = id;
}

Task::~Task()
{
	for (auto node : taskList_) {
		SAFE_DELETE(node);
	}
}

void Task::add(TaskNode *taskNode)
{
	taskList_.push_back(taskNode);
}

void Task::remove(TaskNode *taskNode)
{
	taskList_.remove(taskNode);
}

void Task::process()
{
	while (!_shutdown) {
		std::vector<TaskNode *> deleteNodes;
		for (auto taskNode : taskList_) {
			if (taskNode->expired()) {
				deleteNodes.push_back(taskNode);
				continue;
			}
			taskNode->tick();
		}

		for (auto node : deleteNodes) {
			this->remove(node);
		}
		CONTEXT_SWITCH;
	}
}

void Task::run()
{
	thread_ = MAKE_THREAD(Task, process);
}

//---------------------------------------------------------------------//
TaskManager::TaskManager()
{
	xml_t config;
	if (!loadConfig(&config)) {
		return;
	}
	this->initialize(&config);
}

void TaskManager::initialize(xml_t *config)
{
	xmlNode_t *root = config->FirstChildElement("App")->FirstChildElement("Task");
	if (!root) {
		SLog(L"@ not exist task setting");
		return;
	}
	xmlNode_t *elem = root->FirstChildElement("ThreadCount");
	sscanf_s(elem->GetText(), "%d", &threadCount_);

	for (int i = 0; i < threadCount_; ++i) {
		Task *task = new Task(i);
		taskPool_.push_back(task);
		task->run();
	}
	SLog(L"* task thread, [%d] maked", threadCount_);
}

TaskManager::~TaskManager()
{
	for (auto task : taskPool_) {
		SAFE_DELETE(task);
	}
}

void TaskManager::add(Work *workObject, int freqSec, int durationSec)
{
	const int MINIMAL_THREAD_COUNT = 1;
	if (threadCount_ < MINIMAL_THREAD_COUNT) {
		return;
	}
	static int nodeCount = 0;
	
	TaskNode *node = new TaskNode(workObject, freqSec, durationSec);
	int index = nodeCount % threadCount_;
	Task *task = taskPool_[index];
	task->add(node);

	++nodeCount;
}
