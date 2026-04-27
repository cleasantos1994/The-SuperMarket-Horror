#include "TaskSystem.h"
#include <glm/glm.hpp>
#include <algorithm>

void TaskSystem::InitDay1Tasks() {
    tasks_.clear();
    tasks_.push_back({"RESTOCK_A", "Restock Shelf A", "Put the meme beans back", {2.0f, 0, -5.0f}, false, true, 2.0f});
    tasks_.push_back({"RESTOCK_B", "Restock Shelf B", "The cereal is messy", {-3.0f, 0, -2.0f}, false, false, 2.0f});
    tasks_.push_back({"GREET_1", "Greet Customer", "Say hi to the weird guy", {5.0f, 0, 5.0f}, false, false, 3.0f});
    activeIdx_ = 0;
}

void TaskSystem::InitDay5Tasks() {
    tasks_.clear();
    tasks_.push_back({"RESTOCK_C", "Restock Shelf C", "More work...", {0, 0, -8.0f}, false, true, 2.0f});
    tasks_.push_back({"HELP_CUST", "Help Customer", "They look lost", {-5.0f, 0, 2.0f}, false, false, 3.0f});
    tasks_.push_back({"SCAN", "Scan Items", "Beep beep beep", {0, 0, 0}, false, false, 2.0f});
    activeIdx_ = 0;
}

void TaskSystem::Update(const glm::vec3& playerPos, bool interactPressed) {
    if (activeIdx_ >= tasks_.size()) return;
    
    Task& current = tasks_[activeIdx_];
    float dist = glm::distance(playerPos, current.worldPosition);
    
    if (dist < current.interactRange && interactPressed) {
        current.completed = true;
        current.active = false;
        activeIdx_++;
        if (activeIdx_ < (int)tasks_.size()) {
            tasks_[activeIdx_].active = true;
        }
    }
}

Task* TaskSystem::GetActiveTask() {
    if (activeIdx_ < (int)tasks_.size()) return &tasks_[activeIdx_];
    return nullptr;
}

bool TaskSystem::AllTasksDone() const {
    return activeIdx_ >= (int)tasks_.size();
}

bool TaskSystem::IsNearActiveTask(const glm::vec3& playerPos) const {
    if (activeIdx_ >= (int)tasks_.size()) return false;
    return glm::distance(playerPos, tasks_[activeIdx_].worldPosition) < tasks_[activeIdx_].interactRange;
}

const std::vector<Task>& TaskSystem::GetAllTasks() const { return tasks_; }

std::string TaskSystem::GetCompletionQuip(const std::string& taskId) const {
    return "Good job, wageslave!";
}