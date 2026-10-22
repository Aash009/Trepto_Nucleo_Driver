# Application items. Add a target here only when the module has approved
# requirements, real source and a registered unit test.
#
# Variant rules (software_items.yaml):
#   therapy/common          all variants
#   therapy/treprostinil    RCM_PRODUCT=TREPTOFLO only
#   therapy/insulin         RCM_PRODUCT=INSUFLO only
#   connectivity/           RCM_ENABLE_CONNECTIVITY=ON only (INSUFLO)
# Example once implemented:
#   if(RCM_ENABLE_CONNECTIVITY)
#       add_subdirectory(connectivity)
#   endif()
