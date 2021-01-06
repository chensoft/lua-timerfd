#include <sys/timerfd.h>
#include <lauxlib.h>
#include <unistd.h>

typedef struct {
    int fd;
} timerfd;

static int l_timerfd_create(lua_State *L)
{
    int fd = timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC | TFD_NONBLOCK);
    if (fd == -1)
    {
        lua_pushnil(L);
        lua_pushstring(L, "create timerfd failed");
        return 2;
    }

    ((timerfd*)lua_newuserdata(L, sizeof(timerfd)))->fd = fd;
    luaL_getmetatable(L, "timerfd");
    lua_setmetatable(L, -2);

    return 1;
}

static int l_timerfd_destroy(lua_State *L)
{
    timerfd *tf = (timerfd*)luaL_checkudata(L, 1, "timerfd");
    close(tf->fd);
    return 0;
}

static int l_timerfd_settime(lua_State *L)
{
    timerfd *tf = (timerfd*)luaL_checkudata(L, 1, "timerfd");
    lua_Integer val = luaL_checkinteger(L, 2);

    struct itimerspec ts;
    ts.it_interval.tv_sec  = val / 1000000000;
    ts.it_interval.tv_nsec = val % 1000000000;
    ts.it_value.tv_sec  = ts.it_interval.tv_sec;
    ts.it_value.tv_nsec = ts.it_interval.tv_nsec;

    if (timerfd_settime(tf->fd, 0, &ts, NULL) == -1)
    {
        lua_pushnil(L);
        lua_pushstring(L, "timerfd invalid");
        return 2;
    }

    return 0;
}

static int l_timerfd_gettime(lua_State *L)
{
    timerfd *tf = (timerfd*)luaL_checkudata(L, 1, "timerfd");

    struct itimerspec ts;
    if (timerfd_gettime(tf->fd, &ts) == -1)
    {
        lua_pushnil(L);
        lua_pushstring(L, "timerfd invalid");
        return 2;
    }

    lua_pushinteger(L, ts.it_interval.tv_sec * 1000000000 + ts.it_interval.tv_nsec);
    return 1;
}

static int l_timerfd_pollfd(lua_State *L)
{
    timerfd *tf = (timerfd*)luaL_checkudata(L, 1, "timerfd");
    lua_pushinteger(L, tf->fd);
    return 1;
}

static int l_timerfd_read(lua_State *L)
{
    timerfd *tf = (timerfd*)luaL_checkudata(L, 1, "timerfd");

    lua_Integer times = 0;
    if (read(tf->fd, &times, sizeof(times)) <= 0)
    {
        lua_pushnil(L);
        lua_pushstring(L, "empty");
        return 2;
    }

    lua_pushinteger(L, times);
    return 1;
}

static const struct luaL_Reg l_methods[] = {
        {"settime", l_timerfd_settime},
        {"gettime", l_timerfd_gettime},
        {"pollfd", l_timerfd_pollfd},
        {"read", l_timerfd_read},
        {"close", l_timerfd_destroy},
        {NULL, NULL}
};

static const struct luaL_Reg l_initial[] = {
        {"new", l_timerfd_create},
        {NULL, NULL}
};

int luaopen_timerfd(lua_State *L)
{
    luaL_checkversion(L);

    luaL_newmetatable(L, "timerfd");

    lua_newtable(L);
    luaL_setfuncs(L, l_methods, 0);
    lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, l_timerfd_destroy);
    lua_setfield(L, -2, "__gc");

    luaL_newlib(L, l_initial);
    return 1;
}
