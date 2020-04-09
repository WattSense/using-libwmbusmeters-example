#include <cstdio>
#include <string>
#include <vector>

#include "meters_common_implementation.h"
#include "wmbus_utils.h"

struct PktFwdWMBus : public WMBusCommonImplementation {
    PktFwdWMBus() : WMBusCommonImplementation(WMBusDeviceType::DEVICE_UNKNOWN) {}

    ~PktFwdWMBus() {}

    bool ping() override { return true; }

    uint32_t getDeviceId() override { return 0; }

    LinkModeSet getLinkModes() override { return LinkModeSet(LinkModeBits::C1_bit); }

    LinkModeSet supportedLinkModes() override { return LinkModeSet(LinkModeBits::C1_bit); }

    int numConcurrentLinkModes() override { return 1; }

    bool canSetLinkModes(LinkModeSet lms) override { return supportedLinkModes().hasAll(lms); }

    void setLinkModes(LinkModeSet lms) override {}

    SerialDevice* serial() override { return NULL; }

    void simulate() override {}
};

struct MeterGeneric : public MeterCommonImplementation {
    MeterGeneric(WMBus* bus, MeterInfo& mi)
        : MeterCommonImplementation(bus, mi, MeterType::UNKNOWN, 0) {}

    ~MeterGeneric() {}

   protected:
    void processContent(Telegram* t) {}
};

int main(int argc, char* argv[]) {
    PktFwdWMBus bus;
    std::vector<std::string> shells;
    std::vector<std::string> jsons;
    MeterInfo minfo("generic", "", "37178014", "", LinkModeSet(LinkModeBits::C1_bit), shells,
                    jsons);
    MeterGeneric m(&bus, minfo);
    printf("%d\n", bus.ping());
    return 0;
}
