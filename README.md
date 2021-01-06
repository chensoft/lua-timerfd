# lua-timerfd

Lua bindings for Linux timerfd

## Platform

* Linux (2.6.27+)

## Install

build with cmake or run `luarocks install timerfd`

## Interface

* new()
* settime(nanoseconds)
* gettime()
* pollfd()
* read()
* close()

## Example

```lua
local timerfd = require("timerfd").new()
timerfd:settime(33333333)  -- 30fps

print(string.format("fd %d time %d", timerfd:pollfd(), timerfd:gettime()))

local runloop = require("cqueues").new()
runloop:wrap(function()
    local pollfd = timerfd:pollfd()

    while true do
        local now = require("posix.time").clock_gettime(require("posix.time").CLOCK_REALTIME)
        print(string.format("tick %s.%09d", os.date("%Y-%m-%d %H:%M:%S", now.tv_sec), now.tv_nsec))

        require("cqueues").poll({pollfd = pollfd, events = "r"})
        timerfd:read()
    end
end)    
assert(runloop:loop())
```

## License

lua-timerfd is released under the MIT License. See the LICENSE file for more information.

## Contact

You can contact me by email: admin@chensoft.com.