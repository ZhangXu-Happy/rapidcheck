#pragma once

#include "detail/GeneratorFwd.h"
#include "detail/Results.h"
#include "detail/Property.h"

#include "detail/Check.h"

//! TODO move to separate header
#if defined(SECTION) && defined(FAIL)
#  define RC_HAS_CATCH 1
#else
#  define RC_HAS_CATCH 0
#endif

//! Fails the current test case unless the given condition is `true`.
#define RC_ASSERT(condition) RC_CONDITIONAL_RESULT(Failure, !(condition))

//! Unconditionally fails the current test case with the given message.
#define RC_FAIL(msg) RC_UNCONDITIONAL_RESULT(Failure, (msg))

//! Succeed if the given condition is true.
#define RC_SUCCEED_IF(condition) RC_CONDITIONAL_RESULT(Success, (condition))

//! Unconditionally succeed with the given message.
#define RC_SUCCEED(msg) RC_UNCONDITIONAL_RESULT(Success, msg)

//! Discards the current test case if the given condition is false.
#define RC_PRE(condition) RC_CONDITIONAL_RESULT(Discard, !(condition))

//! Discards the current test case with the given description.
#define RC_DISCARD(msg) RC_UNCONDITIONAL_RESULT(Discard, (msg))

namespace rc {

#if RC_HAS_CATCH == 1

//! For use with `catch.hpp`. Use this function wherever you would use a
//! `SECTION` for convenient checking of properties.
//!
//! @param description  A description of the property.
//! @param testable     The object that implements the property.
//!
//! TODO move to separate header
template<typename Testable>
void prop(const std::string &description, Testable testable)
{
    using namespace detail;

    SECTION(description) {
        auto result = checkProperty(toProperty(testable));
        INFO(resultMessage(result) << "\n");

        FailureResult failure;
        if (result.match(failure)) {
            std::string counterExample;
            for (const auto &desc : failure.counterExample) {
                counterExample += desc.typeName() + ":\n";
                counterExample += desc.stringValue() + "\n\n";
            }
            INFO(counterExample);
            FAIL("Property failed: " << failure.description);
        }
    }
}

#endif

}

#include "detail/Check.hpp"