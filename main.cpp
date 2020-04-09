#include <assert.h>

#include <cstdio>
#include <string>
#include <vector>

#include "dvparser.h"
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
    // FIXME: not called because wmbusmeters overloads it :/
    void handleTelegram(vector<uchar> frame) {
        printf(__FUNCTION__);
        printf("\n");
    }
    void processContent(Telegram* t) override {
        int offset = 0;
        double temp = 0;
        bool ok = extractDVdouble(&t->values, "0265", &offset, &temp);
        printf("Extract: %d\n", ok);
        printf("Temperature is %f\n", temp);
    }
};

static uchar hex_val(char c) {
    if (c > '9') {
        return c - 'a' + 10;
    }
    return c - '0';
}

static std::vector<uchar> hex_to_str(const std::string& str) {
    std::vector<uchar> bytes;
    const char* s = str.data();
    while (*s != 0 && *(s + 1) != 0) {
        uchar high = hex_val(tolower(*s)) & 0xF;
        uchar low = hex_val(tolower(*(s + 1))) & 0xF;
        uchar b = (high << 4) | low;
        bytes.push_back(b);
        s += 2;
    }
    assert(*s == 0);
    return bytes;
}

static bool bus_cb(std::vector<uchar> bytes) {
    printf("Busmon new frame: ");
    for (auto it = bytes.begin(); it != bytes.end(); it++) {
        printf("%02x", *it);
    }
    printf("\n");
}

static void meter_cb(Telegram* t, Meter* m) {
    printf("Meter telegram %s for %s\n", t->id.c_str(), m->ids()[0].c_str());
}

int main(int argc, char* argv[]) {
    debugEnabled(true);
    // WMBus
    PktFwdWMBus bus;
    bus.onTelegram(bus_cb);

    // Meter
    std::vector<std::string> shells;
    std::vector<std::string> jsons;
    MeterInfo minfo("generic", "", "14801737", "", LinkModeSet(LinkModeBits::C1_bit), shells,
                    jsons);
    MeterGeneric m(&bus, minfo);
    m.onUpdate(meter_cb);

    // runtime
    std::vector<uchar> bytes = hex_to_str(argv[1]);
    bus.handleTelegram(bytes);

    return 0;
}
