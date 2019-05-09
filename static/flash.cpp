#include "config.h"
#include "flash.hpp"
#include "activation.hpp"

#include <experimental/filesystem>

namespace
{
constexpr auto PATH_INITRAMFS = "/run/initramfs";
constexpr auto IMAGE_BIOS = "image-bios";
} // namespace

namespace phosphor
{
namespace software
{
namespace updater
{
namespace softwareServer = sdbusplus::xyz::openbmc_project::Software::server;
namespace fs = std::experimental::filesystem;

void Activation::flashWrite()
{
    // For static layout code update, just put images in /run/initramfs.
    // It expects user to trigger a reboot and an updater script will program
    // the image to flash during reboot.
    fs::path uploadDir(IMG_UPLOAD_DIR);
    fs::path toPath(PATH_INITRAMFS);
    fs::copy_file(uploadDir / versionId / IMAGE_BIOS, toPath / IMAGE_BIOS,
                      fs::copy_options::overwrite_existing);
}

void Activation::onStateChanges(sdbusplus::message::message& /*msg*/)
{
    // Empty
    printf("bios update success...\n");

    
    softwareServer::Activation::activation(
        softwareServer::Activation::Activations::Active);

}

} // namespace updater
} // namespace software
} // namespace phosphor
