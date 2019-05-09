#pragma once

#include "activation.hpp"
#include "org/openbmc/Associations/server.hpp"
#include "version.hpp"

#include <sdbusplus/server.hpp>
#include <xyz/openbmc_project/Common/FactoryReset/server.hpp>

namespace phosphor
{
namespace software
{
namespace updater
{

using ItemUpdaterInherit = sdbusplus::server::object::object<
    sdbusplus::xyz::openbmc_project::Common::server::FactoryReset,
    sdbusplus::org::openbmc::server::Associations>;

namespace MatchRules = sdbusplus::bus::match::rules;
using VersionClass = phosphor::software::manager::Version;
using AssociationList =
    std::vector<std::tuple<std::string, std::string, std::string>>;

/** @class ItemUpdater
 *  @brief Manages the activation of the BMC version items.
 */
class ItemUpdater : public ItemUpdaterInherit
{
  public:
    /*
     * @brief Types of Activation status for image validation.
     */
    enum class ActivationStatus
    {
        ready,
        invalid,
        active
    };

    /** @brief Constructs ItemUpdater
     *
     * @param[in] bus    - The D-Bus bus object
     */
    ItemUpdater(sdbusplus::bus::bus& bus, const std::string& path) :
        ItemUpdaterInherit(bus, path.c_str(), false), bus(bus),
        versionMatch(bus,
                     MatchRules::interfacesAdded() +
                         MatchRules::path("/xyz/openbmc_project/software"),
                     std::bind(std::mem_fn(&ItemUpdater::createActivation),
                               this, std::placeholders::_1))
    {
        emit_object_added();
    };

    /**
     * @brief Erase specified entry D-Bus object
     *        if Action property is not set to Active
     *
     * @param[in] entryId - unique identifier of the entry
     */
    void erase(std::string entryId);

    /** @brief Creates an active association to the
     *  newly active software image
     *
     * @param[in]  path - The path to create the association to.
     */
    void createActiveAssociation(const std::string& path);

    /** @brief Removes the associations from the provided software image path
     *
     * @param[in]  path - The path to remove the associations from.
     */
    void removeAssociations(const std::string& path);

    /** @brief Brings the total number of active BMC versions to
     *         ACTIVE_BMC_MAX_ALLOWED -1. This function is intended to be
     *         run before activating a new BMC version. If this function
     *         needs to delete any BMC version(s) it will delete the
     *         version(s) with the highest priority, skipping the
     *         functional BMC version.
     *
     * @param[in] caller - The Activation object that called this function.
     */
    void freeSpace(Activation& caller);

  private:
    /** @brief Callback function for Software.Version match.
     *  @details Creates an Activation D-Bus object.
     *
     * @param[in]  msg       - Data associated with subscribed signal
     */
    void createActivation(sdbusplus::message::message& msg);

    /**
     * @brief Validates the presence of SquashFS image in the image dir.
     *
     * @param[in]  filePath  - The path to the image dir.
     * @param[out] result    - ActivationStatus Enum.
     *                         ready if validation was successful.
     *                         invalid if validation fail.
     *                         active if image is the current version.
     *
     */
    ActivationStatus validateSquashFSImage(const std::string& filePath);

    /** @brief BMC factory reset - marks the read-write partition for
     * recreation upon reboot. */
    void reset() override;

    /** @brief Persistent sdbusplus D-Bus bus connection. */
    sdbusplus::bus::bus& bus;

    /** @brief Persistent map of Activation D-Bus objects and their
     * version id */
    std::map<std::string, std::unique_ptr<Activation>> activations;

    /** @brief Persistent map of Version D-Bus objects and their
     * version id */
    std::map<std::string, std::unique_ptr<VersionClass>> versions;

    /** @brief sdbusplus signal match for Software.Version */
    sdbusplus::bus::match_t versionMatch;

    /** @brief This entry's associations */
    AssociationList assocs = {};
};

} // namespace updater
} // namespace software
} // namespace phosphor
