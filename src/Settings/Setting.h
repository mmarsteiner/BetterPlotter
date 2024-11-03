//
// Created by mmars on 11/06/2024.
//

#pragma once

#include <Utils/stack.h>
#include <Utils/utils.h>

#include <cstdint>
#include <cstring>

#define SETTING_LABEL_MAX 20

#define NEGATE_CHAR 26  // character code associated with TI negate sign

namespace bp {
struct Setting {
    char label[SETTING_LABEL_MAX]{};
    uint8_t editingOffset = 0;  // stores the "character" of the setting value
                                // that the cursor is over
    char tmpEdit[10]{};

    static void MoveCursorCol(int newCol);

    virtual void ProcessInput(sk_key_t key) = 0;

    void StartEditing();

    virtual ~Setting() = default;

    virtual void SetVal(void *in) = 0;

    uint8_t GetScreenLine(char *out);

    uint8_t GetCursorCol() const;

    virtual void GetVal(void *out) = 0;

    double ToDouble() {
        double val;
        GetVal(&val);
        return val;
    }

    virtual void GetValString(char *out) = 0;

    void GetLabel(char *out) const { strncpy(out, label, SETTING_LABEL_MAX); }

    virtual uint16_t GetValSize() = 0;

    virtual Setting *Copy() = 0;

   protected:
    explicit Setting(const char *label) {
        strncpy(this->label, label, SETTING_LABEL_MAX);
    }
};

struct SettingDouble final : Setting {
    double val;

    void ProcessInput(sk_key_t key) override;

    static SettingDouble *Create(const char *label, double val) {
        return new SettingDouble(label, val);
    }

    void GetVal(void *out) override { *static_cast<double *>(out) = val; }

    void GetValString(char *out) override { DoubleToString(val, out); }

    uint16_t GetValSize() override { return sizeof(val); }

    Setting *Copy() override { return Create(label, val); }

    void SetVal(void *in) override { val = *static_cast<double *>(in); }

    ~SettingDouble() override = default;

   private:
    SettingDouble(const char *label, const double val)
        : Setting(label), val{val} {}
};

struct SettingUInt final : Setting {
    uint16_t val;

    void ProcessInput(sk_key_t key) override;

    static SettingUInt *Create(const char *label, uint16_t val) {
        return new SettingUInt(label, val);
    }

    void GetVal(void *out) override { *static_cast<uint16_t *>(out) = val; }
    void GetValString(char *out) override {
        IntToString(static_cast<int>(val), out);
    }
    uint16_t GetValSize() override { return sizeof(val); }

    Setting *Copy() override { return Create(label, val); }

    void SetVal(void *in) override { val = *static_cast<uint16_t *>(in); }

    ~SettingUInt() override = default;

   private:
    SettingUInt(const char *label, const uint16_t val)
        : Setting(label), val{val} {}
};

struct SettingBool final : Setting {
    bool val;

    void ProcessInput(sk_key_t key) override;

    static SettingBool *Create(const char *label, bool val) {
        return new SettingBool(label, val);
    }

    void GetVal(void *out) override { *static_cast<bool *>(out) = val; }
    void GetValString(char *out) override { BoolToString(val, out); }
    uint16_t GetValSize() override { return sizeof(val); }

    Setting *Copy() override { return Create(label, val); }

    void SetVal(void *in) override { val = *static_cast<bool *>(in); }

    ~SettingBool() override = default;

   private:
    SettingBool(const char *label, const bool val) : Setting(label), val{val} {}
};
}  // namespace bp
