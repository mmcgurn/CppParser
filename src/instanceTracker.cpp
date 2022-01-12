#include "instanceTracker.hpp"
#include "factory.hpp"

std::shared_ptr<void> cppParser::InstanceTracker::GetInstancePointer(const std::shared_ptr<Factory>& factory) const {
    if (instances.count(factory->GetClassType())) {
        auto classInstances = instances.at(factory->GetClassType());
        for (const auto& [storedFactory, storedInstance] : classInstances) {
            if (*storedFactory == *factory) {
                return storedInstance;
            }
        }
    }
    return {};
}

void cppParser::InstanceTracker::SetInstancePointer(const std::shared_ptr<Factory>& factory, std::shared_ptr<void> instance) {
    instances[factory->GetClassType()].emplace_back(std::make_pair(factory, instance));
}
