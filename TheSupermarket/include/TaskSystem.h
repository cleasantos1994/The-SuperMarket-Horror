#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>

struct Task {
    std::string id;
    std::string description;
    std::string subText;
    glm::vec3   worldPosition;
    bool        completed;
    bool        active;
    float       interactRange;
};

class TaskSystem {
public:
    void InitDay1Tasks();
    void InitDay5Tasks();
    void Update(const glm::vec3& playerPos, bool interactPressed);
    Task* GetActiveTask();
    bool  AllTasksDone() const;
    bool  IsNearActiveTask(const glm::vec3& playerPos) const;
    const std::vector<Task>& GetAllTasks() const;
    std::string GetCompletionQuip(const std::string& taskId) const;

private:
    std::vector<Task> tasks_;
    int               activeIdx_ = 0;
};
