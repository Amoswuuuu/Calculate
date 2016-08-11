#ifndef __CALCULATE_H__
#define __CALCULATE_H__

#ifdef __cplusplus

#include <queue>
#include <stack>
#include <regex>

#include "calculate/exceptions.hpp"
#include "calculate/symbols.hpp"


namespace calculate {

    using namespace calculate_symbols;
    using namespace calculate_exceptions;

    using vString = std::vector<String>;
    using pValue = std::unique_ptr<double[]>;
    using Regex = std::regex;

    using qSymbol = std::queue<pSymbol>;
    using sSymbol = std::stack<pSymbol>;

    using qEvaluable = std::queue<pEvaluable>;
    using sEvaluable = std::stack<pEvaluable>;


    class Calculate final {
        String _expression;
        vString _variables;
        pValue _values;
        pEvaluable _tree;

        static qSymbol _tokenize(const String &expr, const vString &vars,
                                 const pValue &values);
        static qSymbol _check(qSymbol &&input);
        static qEvaluable _shuntingYard(qSymbol &&infix);
        static pEvaluable _buildTree(qEvaluable &&postfix);

        static vString _extract(const String &vars);
        static vString _validate(const vString &vars);

        Calculate() = delete;

    public:
        Calculate(const Calculate &other);
        Calculate(Calculate &&other);
        Calculate(const String &expr, const String &vars);
        Calculate(const String &expr, const vString &vars={});

        Calculate& operator=(const Calculate &other);
        Calculate& operator=(Calculate &&other);

        bool operator==(const Calculate &other) const noexcept;

        double operator() () const;
        double operator() (double value) const;
        double operator() (vValue values) const;

        template <typename Head, typename... Tail>
        double operator() (Head head, Tail... tail) const {
            if (sizeof...(tail) + 1 > _variables.size())
                throw WrongArgumentsException();
            _values[_variables.size() - sizeof...(tail) - 1] = head;
            return this->operator() (tail...);
        };
        
        const String& getExpression() const noexcept {return _expression;}
        const vString& getVariables() const noexcept {return _variables;}
    };

}

#else

#include "calculate/c-interface.h"

#endif

#endif
