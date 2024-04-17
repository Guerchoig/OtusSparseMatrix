#pragma once
#include <array>
#include <stdarg.h>
#include <memory>
#include <sstream>
#include <map>
#include <tuple>
#include <type_traits>

using Subscript = int;

template <std::size_t N, typename T = int, T Emp = -1, Subscript Key_emp = -1>
class DNmatr
{
public:
    static constexpr T emp = Emp;
    static constexpr Subscript k_emp = Key_emp;

    static_assert(N >= 1, "Nmatrix dimension must be greater than zero!");
    using key_t = std::array<Subscript, N>;

public:
    struct Val
    {

        using Matr = DNmatr<N, T, Emp, Key_emp>;
        Matr *pm;
        key_t key;
        T val;

        Val() {}

        Val(Matr *pm, key_t &key, T val) : pm(pm), key(key), val(val) {}

        friend void swap(Val &first, Val &second)
        {
            using std::swap;
            swap(first.pm, second.pm);
            swap(first.key, second.key);
            swap(first.val, second.val);
        }

        Val(Val &&other) = default;
        Val(Val &other) = default;
        ~Val() = default;

        Val(T _val) : Val()
        {
            val = _val;
            key.fill(emp);
        }

        const Val &operator=(const Val &other)
        {
            // Val temp(other);
            auto this_pm = this->pm;
            if (this->val == emp)
            { // el doesn't exists
                if (other.val != emp)
                { // new val is not empty
                    Val v(this_pm, key, other.val);
                    const Val &res = this_pm->put_in_storage(v);
                    return res;
                }
                return this_pm->v_emp;
            }
            else
            {
                if (other.val == emp)
                {
                    this_pm->erase_from_storage(this->key);
                    return this_pm->v_emp;
                }
                this->val = other.val;
                return *this;
            }
        }

        operator T() const
        {
            return val;
        }
    };

private:
    std::map<key_t, Val> storage_map;

    Val v_emp;
    const Val &put_in_storage(Val &_val) noexcept
    {
        _val.pm = this;
        auto res = storage_map.insert(std::make_pair(_val.key, _val));
        const Val &frst = (res.first)->second;
        return frst;
    }

    const Val &find_in_storage(const Val &_val)
    {
        auto res = storage_map.find(_val.key);
        if (res != storage_map.end())
        {
            const Val &frst = res->second;
            return frst;
        }
        return v_emp;
    }

    void erase_from_storage(key_t key)
    {
        storage_map.erase(key);
    }

public:
    DNmatr()
    {
        key_t key;
        key.fill(emp);
        new (&v_emp) Val(this, key, emp);
    }

    template <typename... Subscript>
    Val &operator()(Subscript... subscr)
    {
        key_t key = {subscr...};

        auto res = storage_map.find(key);
        if (res == storage_map.end())
        {
            v_emp.key = key;
            return v_emp;
        }

        return res->second;
    }

    std::map<key_t, Val>::iterator begin() { return storage_map.begin(); }
    std::map<key_t, Val>::iterator end() { return storage_map.end(); }
    
    size_t size()
    {
        return storage_map.size();
    }
};
