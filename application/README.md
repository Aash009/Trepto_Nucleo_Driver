# Application layer

Product behaviour. One sub-folder per software item from FAD Table 8; the item and
class of every file is set in ../software_items.yaml.

| Folder | Item | Class |
|---|---|---|
| sdc/ | SI-P-01 Safe Delivery Core | C |
| therapy/ | SI-P-02 Therapy Policy | C |
| alarm_manager/ | SI-P-04 Alarm manager | C |
| gui/ | SI-P-06 GUI | B (proposed, open on I-06) |
| connectivity/ | SI-P-07 Connectivity (Insuflo build only) | B (proposed) |
| logging/ | SI-P-08 Logging, history export | B (proposed, open on I-06) |

Dependency rules (enforced by tools/ci/check_layering.py):
- May include system/ headers. May include driver/ headers only for the edges listed
  in tools/ci/layer_rules.yaml (FAD Table 19 interface ownership).
- A Class B folder may include a Class C header only from an `api/` folder
  (sdc/api, alarm_manager/api, system/journal/api) or from a system/driver module
  `inc/` that has no `api/` folder.
- A Class C folder never includes a Class B header.
- No direct HAL access. All product .c/.h files here are design placeholders.
