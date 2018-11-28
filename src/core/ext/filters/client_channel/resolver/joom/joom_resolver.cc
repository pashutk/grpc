//
// Created by Evgeny Savinov on 11/19/18.
//

#include "src/core/ext/filters/client_channel/resolver_registry.h"

const char *JOOM_RESOLVER_SCHEME = "joom";

namespace grpc_core {

namespace {

class JoomResolverFactory : public ResolverFactory {
 public:
  OrphanablePtr<Resolver> CreateResolver(
      const ResolverArgs &args) const override {

    auto balancer_path = args.uri->authority;

    if (GPR_UNLIKELY(0 == strcmp(balancer_path, ""))) {
      gpr_log(GPR_ERROR, "empty balancer path");
      return OrphanablePtr<Resolver>(nullptr);
    }

    return grpc_core::ResolverRegistry::CreateResolver(balancer_path, args.args, args.pollset_set,
                                                       args.combiner);
  }

  const char *scheme() const override { return JOOM_RESOLVER_SCHEME; }
};

}  // namespace

}  // namespace grpc_core

void grpc_resolver_joom_init() {

  grpc_core::ResolverRegistry::Builder::InitRegistry();
  grpc_core::ResolverFactory *existing_factory =
      grpc_core::ResolverRegistry::LookupResolverFactory(JOOM_RESOLVER_SCHEME);

  if (existing_factory == nullptr) {
    grpc_core::ResolverRegistry::Builder::RegisterResolverFactory(
        grpc_core::UniquePtr<grpc_core::ResolverFactory>(
            grpc_core::New<grpc_core::JoomResolverFactory>()));
  }

}

void grpc_resolver_joom_shutdown() {}