// During SUBSET compilation, Makefile.common settings will use this file to resolve
// the transitive dependency issue of 'retro_pgm2_cards.cpp' in libretro.cpp.

#include "retro_pgm2_cards.h"

void retro_pgm2_cards_reset() {}
void retro_pgm2_cards_push_options(std::vector<const retro_core_option_v2_definition*>& vars_systems) {}
void retro_pgm2_cards_refresh_environment() {}
void retro_pgm2_cards_apply_variables() {}
void retro_pgm2_cards_save_files() {}
void retro_pgm2_cards_after_set_environment(void) {}
