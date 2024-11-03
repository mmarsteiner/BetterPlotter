//
// Created by Matthew on 6/06/2024.
//

#pragma once

namespace bp {

class MenuItem {
  public:
    virtual ~MenuItem() = default;
    virtual bool Run() = 0;
    virtual void GetLabel(char *out) = 0;
    virtual void AppendLabel(char *out) = 0;
};

}  // namespace bp
