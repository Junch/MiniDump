#pragma once
#include <type_traits>
#include <utility>

namespace RAII
{
    template < typename RollbackLambda >
    class ScopeGuard;

    template < typename RollbackLambda >
    auto  make_ScopeGuard(RollbackLambda &&r)->ScopeGuard<typename
        std::decay<RollbackLambda>::type>;

    template < typename RollbackLambda >
    class ScopeGuard
    {
        // The input may have any of: cv-qualifiers, l-value reference, or both;
        // so I don't do an exact template match.  I want the return to be just
        // "ScopeGuard," but I can't figure it out right now, so I'll make every
        // version a friend.
        template < typename AnyRollbackLambda >
        friend
            auto make_ScopeGuard(AnyRollbackLambda &&)->ScopeGuard<typename
            std::decay<AnyRollbackLambda>::type>;

    public:
        using lambda_type = RollbackLambda;

    private:
        // Keep the lambda, of course, and if you really need it at the end
        bool        committed;
        lambda_type  rollback;

        // Keep the main constructor private so regular creation goes through the
        // external function.
        explicit  ScopeGuard(lambda_type rollback_action)
            : committed{ false }, rollback{ std::move(rollback_action) }
        {}

    public:
        // Do allow moves
        ScopeGuard(ScopeGuard &&that)
            : committed{ that.committed }, rollback{ std::move(that.rollback) }
        {
            that.committed = true;
        }
        ScopeGuard(ScopeGuard const &) = delete;

        // Cancel the roll-back from being called.
        void  commit()  { committed = true; }

        // The magic happens in the destructor.
        // (Too bad that there's still no way, AFAIK, to reliably check if you're
        // already in exception-caused stack unwinding.  For now, we just hope the
        // roll-back doesn't throw.)
        ~ScopeGuard()  { if (!committed) rollback(); }
    };

    template < typename RollbackLambda >
    auto  make_ScopeGuard(RollbackLambda &&r) -> ScopeGuard<typename
        std::decay<RollbackLambda>::type>
    {
        using std::forward;

        return ScopeGuard<typename std::decay<RollbackLambda>::type>{
            forward<RollbackLambda>(r) };
    }

    template < typename ActionLambda, typename RollbackLambda >
    auto  make_ScopeGuard(ActionLambda && a, RollbackLambda &&r, bool
        roll_back_if_action_throws) -> ScopeGuard<typename
        std::decay<RollbackLambda>::type>
    {
        using std::forward;

        if (not roll_back_if_action_throws)  forward<ActionLambda>(a)();
        auto  result = make_ScopeGuard(forward<RollbackLambda>(r));
        if (roll_back_if_action_throws)  forward<ActionLambda>(a)();
        return result;
    }
}
