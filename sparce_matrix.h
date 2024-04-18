/// @brief The file contains h-only lib, implementing an N-dim infinite sparce matrix
/// based upon an std::map container
#pragma once
#include <array>
#include <map>
#include <tuple>

/// @brief Type for matrix subscripts
using Subscript = int;

/// @brief Main N-dim matrix class, based upon an std::map container
/// @tparam T -the type to be stored
/// @tparam N - nof dimensions
/// @tparam Emp - empty value
template <std::size_t N, typename T = int, T Emp = -1>
class DNmatr
{
private:
    static_assert(N >= 1, "Nmatrix dimension must be greater than zero!");

    /// @brief A shortcut for the matrix class
    using Matr = DNmatr<N, T, Emp>;

    /// @brief Class to implement keys of the matrix
    using key_t = std::array<Subscript, N>;

public:
    /// @brief Lower-case empty value variable
    static constexpr T emp = Emp;

private:
    /// @brief A class to operate temporary 'intermediate' nodes,
    /// containing a value, a key and a ptr, pointing to the main matrix
    /// Make it possible for Val operator= to access methods of values storage:
    /// - put_in_storage
    /// - erase_from_storage
    struct Val
    {
        key_t key;
        Matr *pm;
        T val;

        Val(Val &&other) = default;
        Val(Val &other) = default;
        ~Val() = default;

        /// @brief Main wirking constructor
        /// @param key matrix key
        /// @param pm  a ptr to the parent matrix
        /// @param val value to store
        Val(key_t &key, Matr *pm = nullptr, T val = emp) : key(key), pm(pm), val(val) {}

        /// @brief Default constructor make it sure the member items are filled with 'emp' and other default values
        Val() : pm(nullptr), val(emp)
        {
            key.fill(emp);
        }

        /// @brief Constructs Val from storage value
        /// @param val value to store
        Val(T val) : pm(nullptr), val(val)
        {
            key.fill(emp);
        }

        /// @brief Operator = stores new non-emty values under subscripts, implicitly received from operator ()
        /// or erases matrix element having subscripts, implicitly received from operator ()
        /// or updates an existing stored value with the new one
        /// @param other a Val containing new value
        /// @return a Val created or updated or an empty-valued Val
        Val operator=(const Val other)
        {

            auto this_pm = this->pm;

            if (this->val == emp)
            { // The el doesn't exists
                if (other.val != emp)
                { // So the new val is not empty, we are storing it
                    Val v(key, this_pm, other.val);
                    auto res = this_pm->put_in_storage(v);
                    const Val ret = Val(key, nullptr, res);
                    return ret;
                }
                const Val ret; // all member requisites are filled with default values
                return ret;
            }
            else
            {
                if (other.val == emp)
                { // We received an empty value and must erase the item under this->key,
                  // if we find one
                    this_pm->erase_from_storage(this->key);
                    const Val ret;
                    return ret;
                }
                // Item not found - doing nothing
                this->val = other.val;
                return *this;
            }
        }

        /// @brief Converts Val into T
        operator T() const
        {
            return val;
        }
    };

    /// @brief Storage of values, subscripts stored in keys
    std::map<key_t, T> storage_map;

    /// @brief Inserts the value into storage
    /// @param _val value to insert
    /// @return stored value
    const T &put_in_storage(Val _val) noexcept
    {
        auto res = storage_map.insert(std::make_pair(_val.key, _val.val));
        const T &ret = (res.first)->second;
        return ret;
    }

    /// @brief Erases the value under provided subscripts from storage
    /// @param key The key, containing subscripts
    void erase_from_storage(key_t key)
    {
        storage_map.erase(key);
    }

public:
    /// @brief N-dim operator()
    /// @tparam ...R subscripts, number and values of which are automathically derived from the calling expression
    /// @param ...subscr delivers Rs into function
    /// @return Val, found under subscripts, or temporary Val, having key, filled with subscripts
    template <typename... R>
    Val operator()(R... subscr)
    {
        key_t key = {subscr...};

        auto res = storage_map.find(key);
        if (res == storage_map.end())
        {
            Val ret(key, this);
            return ret;
        }

        return Val(key, this, res->second);
    }
    /// @brief Makes map's iterator begin() accessible from outside of matrix
    /// @return the iterator to the underlayng std::map
    std::map<key_t, T>::iterator begin() { return storage_map.begin(); }

    /// @brief Makes map's iterator end() accessible from outside of matrix
    /// @return the iterator to the underlayng std::map
    std::map<key_t, T>::iterator end() { return storage_map.end(); }

    /// @brief Returns the number of non-empty elements of matrix
    std::size_t size()
    {
        return storage_map.size();
    }
};
