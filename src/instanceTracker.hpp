#ifndef CPPPARSER_INSTANCETRACKER_HPP
#define CPPPARSER_INSTANCETRACKER_HPP

#include <map>
#include <string>
#include <utility>
#include <vector>
namespace cppParser {

class Factory;

/**
 * Used to track and re use instances for the same factories
 */
class InstanceTracker {
   private:
    std::map<std::string, std::vector<std::pair<std::shared_ptr<Factory>, std::shared_ptr<void>>>> instances;

    std::shared_ptr<void> GetInstancePointer(const std::shared_ptr<Factory>& factory) const;
    void SetInstancePointer(const std::shared_ptr<Factory>& factory, std::shared_ptr<void>);

   public:
    /**
     * Check to see if this factory has been used to create an instance before.  If so return it
     * @tparam Interface
     * @param factory
     * @return
     */
    template <typename Interface>
    std::shared_ptr<Interface> GetInstance(const std::shared_ptr<Factory>& factory) const {
        auto instance = GetInstancePointer(factory);
        if (instance) {
            return std::static_pointer_cast<Interface>(instance);
        }
        return {};
    }

    /**
     * Store the instance to this factory
     * @param factory
     */
    template <typename Interface>
    void SetInstance(const std::shared_ptr<Factory>& factory, std::shared_ptr<Interface> instance) {
        SetInstancePointer(factory, instance);
    }
};
}  // namespace cppParser

#endif  // CPPPARSER_INSTANCETRACKER_HPP
