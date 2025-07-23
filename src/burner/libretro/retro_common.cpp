#include "retro_common.h"
#include "retro_input.h"

struct RomBiosInfo neogeo_bioses[] = {
	{"sp-s3.sp1",         0x91b64be3, 0x00, "MVS Asia/Europe ver. 6 (1 slot)", NEOGEO_MVS | NEOGEO_EUR, 0 },
	{"sp-s2.sp1",         0x9036d879, 0x01, "MVS Asia/Europe ver. 5 (1 slot)", NEOGEO_MVS | NEOGEO_EUR, 0 },
	{"sp-s.sp1",          0xc7f2fa45, 0x02, "MVS Asia/Europe ver. 3 (4 slot)", NEOGEO_MVS | NEOGEO_EUR, 0 },
	{"sp-45.sp1",         0x03cc9f6a, 0x0b, "NEO-MVH MV1C (Asia)"            , NEOGEO_MVS | NEOGEO_EUR, 0 },
	{"sp-u2.sp1",         0xe72943de, 0x03, "MVS USA ver. 5 (2 slot)"        , NEOGEO_MVS | NEOGEO_USA, 0 },
	{"sp1-u2",            0x62f021f4, 0x04, "MVS USA ver. 5 (4 slot)"        , NEOGEO_MVS | NEOGEO_USA, 0 },
	{"sp-e.sp1",          0x2723a5b5, 0x05, "MVS USA ver. 5 (6 slot)"        , NEOGEO_MVS | NEOGEO_USA, 0 },
	{"sp1-u4.bin",        0x1179a30f, 0x06, "MVS USA (U4)"                   , NEOGEO_MVS | NEOGEO_USA, 0 }, 
	{"sp1-u3.bin",        0x2025b7a2, 0x07, "MVS USA (U3)"                   , NEOGEO_MVS | NEOGEO_USA, 0 },
	{"vs-bios.rom",       0xf0e8f27d, 0x08, "MVS Japan ver. 6 (? slot)"      , NEOGEO_MVS | NEOGEO_JAP, 0 },
	{"sp-j2.sp1",         0xacede59C, 0x09, "MVS Japan ver. 5 (? slot)"      , NEOGEO_MVS | NEOGEO_JAP, 0 },
	{"sp1.jipan.1024",    0x9fb0abe4, 0x0a, "MVS Japan ver. 3 (4 slot)"      , NEOGEO_MVS | NEOGEO_JAP, 0 },
	{"sp-j3.sp1",         0x486cb450, 0x0c, "NEO-MVH MV1C (Japan)"           , NEOGEO_MVS | NEOGEO_JAP, 0 },
	{"japan-j3.bin",      0xdff6d41f, 0x0d, "MVS Japan (J3)"                 , NEOGEO_MVS | NEOGEO_JAP, 0 },
	{"sp1-j3.bin",        0xfbc6d469, 0x0e, "MVS Japan (J3, alt)"            , NEOGEO_MVS | NEOGEO_JAP, 0 },
	{"neo-epo.bin",       0xd27a71f1, 0x10, "AES Asia"                       , NEOGEO_AES | NEOGEO_EUR, 0 },
	{"neo-po.bin",        0x16d0c132, 0x0f, "AES Japan"                      , NEOGEO_AES | NEOGEO_JAP, 0 },
	{"uni-bios_4_0.rom",  0xa7aab458, 0x13, "Universe BIOS ver. 4.0"         , NEOGEO_UNI,              0 },
	{"uni-bios_3_3.rom",  0x24858466, 0x14, "Universe BIOS ver. 3.3"         , NEOGEO_UNI,              0 },
	{"uni-bios_3_2.rom",  0xa4e8b9b3, 0x15, "Universe BIOS ver. 3.2"         , NEOGEO_UNI,              0 },
	{"uni-bios_3_1.rom",  0x0c58093f, 0x16, "Universe BIOS ver. 3.1"         , NEOGEO_UNI,              0 },
	{"uni-bios_3_0.rom",  0xa97c89a9, 0x17, "Universe BIOS ver. 3.0"         , NEOGEO_UNI,              0 },
	{"uni-bios_2_3.rom",  0x27664eb5, 0x18, "Universe BIOS ver. 2.3"         , NEOGEO_UNI,              0 },
	{"uni-bios_2_3o.rom", 0x601720ae, 0x19, "Universe BIOS ver. 2.3 (alt)"   , NEOGEO_UNI,              0 },
	{"uni-bios_2_2.rom",  0x2d50996a, 0x1a, "Universe BIOS ver. 2.2"         , NEOGEO_UNI,              0 },
	{"uni-bios_2_1.rom",  0x8dabf76b, 0x1b, "Universe BIOS ver. 2.1"         , NEOGEO_UNI,              0 },
	{"uni-bios_2_0.rom",  0x0c12c2ad, 0x1c, "Universe BIOS ver. 2.0"         , NEOGEO_UNI,              0 },
	{"uni-bios_1_3.rom",  0xb24b44a0, 0x1d, "Universe BIOS ver. 1.3"         , NEOGEO_UNI,              0 },
	{"uni-bios_1_2.rom",  0x4fa698e9, 0x1e, "Universe BIOS ver. 1.2"         , NEOGEO_UNI,              0 },
	{"uni-bios_1_2o.rom", 0xe19d3ce9, 0x1f, "Universe BIOS ver. 1.2 (alt)"   , NEOGEO_UNI,              0 },
	{"uni-bios_1_1.rom",  0x5dda0d84, 0x20, "Universe BIOS ver. 1.1"         , NEOGEO_UNI,              0 },
	{"uni-bios_1_0.rom",  0x0ce453a0, 0x21, "Universe BIOS ver. 1.0"         , NEOGEO_UNI,              0 },
	{NULL,                0,          0,    NULL,                                                       0 }
};

std::vector<dipswitch_core_option> dipswitch_core_options;
struct GameInp *pgi_reset;
struct GameInp *pgi_diag;
struct GameInp *pgi_debug_dip_1;
struct GameInp *pgi_debug_dip_2;
bool bIsNeogeoCartGame                = false;
bool allow_neogeo_mode                = true;
bool neogeo_use_specific_default_bios = false;
bool bAllowDepth32                    = false;
bool bPatchedRomsetsEnabled           = true;
bool bAllowIgnoreCrc                  = false;
bool bLibretroSupportsAudioBuffStatus = false;
bool bLowPassFilterEnabled            = false;
UINT32 nVerticalMode                  = 0;
UINT32 nFrameskip                     = 1;
INT32 g_audio_samplerate              = 48000;
UINT32 nMemcardMode                   = 0;
UINT32 nLightgunCrosshairEmulation    = 0;
UINT8 *diag_input;
uint32_t g_opt_neo_geo_mode           = 0;

#ifdef USE_CYCLONE
// 0 - c68k, 1 - m68k
// we don't use cyclone by default because it breaks savestates cross-platform compatibility (including netplay)
int nSekCpuCore = 1;
static bool bCycloneEnabled = false;
#endif

static UINT8 diag_input_start[]      = { RETRO_DEVICE_ID_JOYPAD_START,  RETRO_DEVICE_ID_JOYPAD_EMPTY };
static UINT8 diag_input_start_a_b[]  = { RETRO_DEVICE_ID_JOYPAD_START,  RETRO_DEVICE_ID_JOYPAD_A, RETRO_DEVICE_ID_JOYPAD_B, RETRO_DEVICE_ID_JOYPAD_EMPTY };
static UINT8 diag_input_start_l_r[]  = { RETRO_DEVICE_ID_JOYPAD_START,  RETRO_DEVICE_ID_JOYPAD_L, RETRO_DEVICE_ID_JOYPAD_R, RETRO_DEVICE_ID_JOYPAD_EMPTY };
static UINT8 diag_input_select[]     = { RETRO_DEVICE_ID_JOYPAD_SELECT, RETRO_DEVICE_ID_JOYPAD_EMPTY };
static UINT8 diag_input_select_a_b[] = { RETRO_DEVICE_ID_JOYPAD_SELECT, RETRO_DEVICE_ID_JOYPAD_A, RETRO_DEVICE_ID_JOYPAD_B, RETRO_DEVICE_ID_JOYPAD_EMPTY };
static UINT8 diag_input_select_l_r[] = { RETRO_DEVICE_ID_JOYPAD_SELECT, RETRO_DEVICE_ID_JOYPAD_L, RETRO_DEVICE_ID_JOYPAD_R, RETRO_DEVICE_ID_JOYPAD_EMPTY };

// Global core options
static const struct retro_core_option_v2_definition var_empty = { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL };
static struct retro_core_option_v2_definition var_fbneo_allow_depth_32 = {
	"fbneo-allow-depth-32",
	"Use 32-bits color depth when available",
	NULL,
	"Change pixel format, some games require this to render properly, it could impact performances on some platforms",
	NULL,
	NULL,
	{
		{ "disabled", NULL },
		{ "enabled",  NULL },
		{ NULL,       NULL },
	},
	"enabled"
};
static struct retro_core_option_v2_definition var_fbneo_vertical_mode = {
	"fbneo-vertical-mode",
	"Vertical mode",
	NULL,
	"Rotate display for vertical screens",
	NULL,
	NULL,
	{
		{ "disabled",       NULL },
		{ "enabled",        NULL },
		{ "alternate",      NULL },
		{ "TATE",           NULL },
		{ "TATE alternate", NULL },
		{ NULL,             NULL },
	},
	"disabled"
};
static struct retro_core_option_v2_definition var_fbneo_force_60hz = {
	"fbneo-force-60hz",
	"Force 60Hz",
	NULL,
	"Ignore game's original refresh rate and try to run it at 60hz instead. It will cause incorrect game speed and frame pacing. It will try to use your monitor's correct refresh rate instead of 60hz if this refresh rate is between 59hz and 61hz.",
	NULL,
	NULL,
	{
		{ "disabled", NULL },
		{ "enabled",  NULL },
		{ NULL,       NULL },
	},
	"disabled"
};
static struct retro_core_option_v2_definition var_fbneo_fixed_frameskip = {
	"fbneo-fixed-frameskip",
	"Fixed Frameskip",
	NULL,
	"When 'Frameskip' is set to 'Fixed', or if the frontend doesn't support the alternative 'Frameskip' mode, skip rendering at a fixed rate of X frames out of X+1",
	NULL,
	"frameskip",
	{
		{ "0",  "No skipping"                         },
		{ "1",  "Skip rendering of 1 frames out of 2" },
		{ "2",  "Skip rendering of 2 frames out of 3" },
		{ "3",  "Skip rendering of 3 frames out of 4" },
		{ "4",  "Skip rendering of 4 frames out of 5" },
		{ "5",  "Skip rendering of 5 frames out of 6" },
		{ NULL, NULL                                  },
	},
	"0"
};
static struct retro_core_option_v2_definition var_fbneo_frameskip_type = {
	"fbneo-frameskip-type",
	"Frameskip",
	NULL,
	"Skip frames to avoid audio buffer under-run (crackling). Improves performance at the expense of visual smoothness. 'Auto' skips frames when advised by the frontend. 'Manual' uses the 'Frameskip Threshold (%)' setting. 'Fixed' uses the 'Fixed Frameskip' setting.",
	NULL,
	"frameskip",
	{
		{ "disabled", NULL },
		{ "Fixed",    NULL },
		{ "Auto",     NULL },
		{ "Manual",   NULL },
		{ NULL,       NULL },
	},
	"disabled"
};
static struct retro_core_option_v2_definition var_fbneo_frameskip_manual_threshold = {
	"fbneo-frameskip-manual-threshold",
	"Frameskip Threshold (%)",
	NULL,
	"When 'Frameskip' is set to 'Manual', specifies the audio buffer occupancy threshold (percentage) below which frames will be skipped. Higher values reduce the risk of crackling by causing frames to be dropped more frequently.",
	NULL,
	"frameskip",
	{
		{ "15", NULL },
		{ "18", NULL },
		{ "21", NULL },
		{ "24", NULL },
		{ "27", NULL },
		{ "30", NULL },
		{ "33", NULL },
		{ "36", NULL },
		{ "39", NULL },
		{ "42", NULL },
		{ "45", NULL },
		{ "48", NULL },
		{ "51", NULL },
		{ "54", NULL },
		{ "57", NULL },
		{ "60", NULL },
		{ NULL, NULL },
	},
	"33"
};
static struct retro_core_option_v2_definition var_fbneo_cpu_speed_adjust = {
	"fbneo-cpu-speed-adjust",
	"CPU clock",
	NULL,
	"Change emulated cpu frequency for various systems, by increasing you can fix native slowdowns in some games, by decreasing you can help performance on low-end devices",
	NULL,
	NULL,
	{
		PERCENT_VALUES
	},
	"100%"
};
static struct retro_core_option_v2_definition var_fbneo_diagnostic_input = {
	"fbneo-diagnostic-input",
	"Diagnostic Input",
	NULL,
	"Configure button combination to enter cabinet service menu",
	NULL,
	NULL,
	{
		{ "None",                NULL },
		{ "Hold Start",          NULL },
		{ "Start + A + B",       NULL },
		{ "Hold Start + A + B",  NULL },
		{ "Start + L + R",       NULL },
		{ "Hold Start + L + R",  NULL },
		{ "Hold Select",         NULL },
		{ "Select + A + B",      NULL },
		{ "Hold Select + A + B", NULL },
		{ "Select + L + R",      NULL },
		{ "Hold Select + L + R", NULL },
		{ NULL,                  NULL },
	},
	"Hold Start"
};
static struct retro_core_option_v2_definition var_fbneo_hiscores = {
	"fbneo-hiscores",
	"Hiscores",
	NULL,
	"Enable high scores support, you also need the file hiscore.dat in your system/fbneo/ folder",
	NULL,
	NULL,
	{
		{ "disabled", NULL },
		{ "enabled",  NULL },
		{ NULL,       NULL },
	},
	"enabled"
};
static struct retro_core_option_v2_definition var_fbneo_allow_patched_romsets = {
	"fbneo-allow-patched-romsets",
	"Allow patched romsets",
	NULL,
	"Allow romsets from your system/fbneo/patched/ folder to override your romsets, crcs will be ignored but sizes and names must still match, you need to close content for this setting to take effect",
	NULL,
	NULL,
	{
		{ "disabled", NULL },
		{ "enabled",  NULL },
		{ NULL,       NULL },
	},
	"enabled"
};
static struct retro_core_option_v2_definition var_fbneo_allow_ignore_crc = {
	"fbneo-allow-ignore-crc",
	"Allow Ignore CRC",
	NULL,
	"Disable CRC checking, match the file name and file size . Enable CRC checking, match the file CRC and file size.",
	NULL,
	NULL,
	{
		{ "disabled", NULL },
		{ "enabled",  NULL },
		{ NULL,       NULL },
	},
	"disabled"
};
static struct retro_core_option_v2_definition var_fbneo_samplerate = {
	"fbneo-samplerate",
	"Samplerate",
	NULL,
	"Configure samplerate, it could impact performances, closing & starting game again is required",
	NULL,
	"audio",
	{
		{ "44100", NULL },
		{ "48000", NULL },
		{ NULL,    NULL },
	},
	"48000"
};
static struct retro_core_option_v2_definition var_fbneo_sample_interpolation = {
	"fbneo-sample-interpolation",
	"Sample Interpolation",
	NULL,
	"Configure sample interpolation, it could impact performances",
	NULL,
	"audio",
	{
		{ "disabled",          NULL },
		{ "2-point 1st order", NULL },
		{ "4-point 3rd order", NULL },
		{ NULL,                NULL },
	},
	"4-point 3rd order"
};
static struct retro_core_option_v2_definition var_fbneo_fm_interpolation = {
	"fbneo-fm-interpolation",
	"FM Interpolation",
	NULL,
	"Configure FM interpolation, it could impact performances",
	NULL,
	"audio",
	{
		{ "disabled",          NULL },
		{ "4-point 3rd order", NULL },
		{ NULL,                NULL },
	},
	"4-point 3rd order"
};
static struct retro_core_option_v2_definition var_fbneo_lowpass_filter = {
	"fbneo-lowpass-filter",
	"LowPass Filter",
	NULL,
	"Enable LowPass Filter",
	NULL,
	"audio",
	{
		{ "disabled", NULL },
		{ "enabled",  NULL },
		{ NULL,       NULL },
	},
	"disabled"
};
static struct retro_core_option_v2_definition var_fbneo_analog_speed = {
	"fbneo-analog-speed",
	"Analog Speed",
	NULL,
	"Mitigate analog controls speed, some games might require low values to be playable",
	NULL,
	NULL,
	{
		PERCENT_VALUES
	},
	"100%"
};
// note : socd is made global for all users, standalone is handling different modes for each user but we really don't want this here... 
//        libretro doesn't really support multiple keyboard users and this setting is mostly (only ?) useful for keyboard users...
static struct retro_core_option_v2_definition var_fbneo_socd = {
	"fbneo-socd",
	"SOCD Setting",
	NULL,
	"Change ULDR priority, mostly useful for keyboard users",
	NULL,
	NULL,
	{
		{ "0",  "disabled" },
		{ "1",  "Simultaneous Neutral" },
		{ "2",  "Last Input Priority (4 Way)" },
		{ "3",  "Last Input Priority (8 Way)" },
		{ "4",  "First Input Priority" },
		{ "5",  "Up Priority (Up-override Down)" },
		{ "6",  "Down Priority (Left/Right Last Input Priority)" },
		{ NULL, NULL },
	},
	"3"
};
static struct retro_core_option_v2_definition var_fbneo_lightgun_crosshair_emulation = {
	"fbneo-lightgun-crosshair-emulation",
	"Crosshair emulation",
	NULL,
	"Change emulated crosshair behavior, \"hide with lightgun device\" will also hide it on touchscreen device",
	NULL,
	NULL,
	{
		{ "hide with lightgun device", NULL },
		{ "always hide",               NULL },
		{ "always show",               NULL },
		{ NULL,                        NULL },
	},
	"hide with lightgun device"
};
#ifdef USE_CYCLONE
static struct retro_core_option_v2_definition var_fbneo_cyclone = {
	"fbneo-cyclone",
	"Enable cyclone",
	NULL,
	"Use at your own risk, it could improve performance on some emulated systems for low-end devices, but there are known side effects : savestates won't be compatible with normal interpreter, and some systems won't work",
	NULL,
	NULL,
	{
		{ "disabled", NULL },
		{ "enabled",  NULL },
		{ NULL,       NULL },
	},
	"disabled"
};
#endif

// Neo Geo core options
static struct retro_core_option_v2_definition var_fbneo_neogeo_mode = {
	"fbneo-neogeo-mode",
	"Neo-Geo mode",
	NULL,
	"Load appropriate bios depending on your choice, under the condition such a bios is compatible with the running game",
	NULL,
	"neogeo",
	{
		{ "DIPSWITCH", "Use bios set in BIOS dipswitch" },
		{ "MVS_EUR",   "MVS Europe/Asia (English)"      },
		{ "MVS_USA",   "MVS USA (English - Censored)"   },
		{ "MVS_JAP",   "MVS Japan (Japanese)"           },
		{ "AES_EUR",   "AES Europe/Asia (English)"      },
		{ "AES_JAP",   "AES Japan (Japanese)"           },
		{ "UNIBIOS",   "UNIBIOS"                        },
		{ NULL, NULL                                    },
	},
	"DIPSWITCH"
};
static struct retro_core_option_v2_definition var_fbneo_memcard_mode = {
	"fbneo-memcard-mode",
	"Memory card mode",
	NULL,
	"Change the behavior for the memory card",
	NULL,
	"neogeo",
	{
		{ "disabled", NULL },
		{ "shared",   NULL },
		{ "per-game", NULL },
		{ NULL,       NULL },
	},
	"disabled"
};
static struct retro_core_option_v2_definition var_fbneo_debug_dip_1_1 = {
	"fbneo-debug-dip-1-1",
	"Debug Dip 1_1",
	NULL,
	"Enable Debug Dip 1_1",
	NULL,
	"neogeo",
	{
		{ "disabled", NULL },
		{ "enabled",  NULL },
		{ NULL,       NULL },
	},
	"disabled"
};
static struct retro_core_option_v2_definition var_fbneo_debug_dip_1_2 = {
	"fbneo-debug-dip-1-2",
	"Debug Dip 1_2",
	NULL,
	"Enable Debug Dip 1_2",
	NULL,
	"neogeo",
	{
		{ "disabled", NULL },
		{ "enabled",  NULL },
		{ NULL,       NULL },
	},
	"disabled"
};
static struct retro_core_option_v2_definition var_fbneo_debug_dip_1_3 = {
	"fbneo-debug-dip-1-3",
	"Debug Dip 1_3",
	NULL,
	"Enable Debug Dip 1_3",
	NULL,
	"neogeo",
	{
		{ "disabled", NULL },
		{ "enabled",  NULL },
		{ NULL,       NULL },
	},
	"disabled"
};
static struct retro_core_option_v2_definition var_fbneo_debug_dip_1_4 = {
	"fbneo-debug-dip-1-4",
	"Debug Dip 1_4",
	NULL,
	"Enable Debug Dip 1_4",
	NULL,
	"neogeo",
	{
		{ "disabled", NULL },
		{ "enabled",  NULL },
		{ NULL,       NULL },
	},
	"disabled"
};
static struct retro_core_option_v2_definition var_fbneo_debug_dip_1_5 = {
	"fbneo-debug-dip-1-5",
	"Debug Dip 1_5",
	NULL,
	"Enable Debug Dip 1_5",
	NULL,
	"neogeo",
	{
		{ "disabled", NULL },
		{ "enabled",  NULL },
		{ NULL,       NULL },
	},
	"disabled"
};
static struct retro_core_option_v2_definition var_fbneo_debug_dip_1_6 = {
	"fbneo-debug-dip-1-6",
	"Debug Dip 1_6",
	NULL,
	"Enable Debug Dip 1_6",
	NULL,
	"neogeo",
	{
		{ "disabled", NULL },
		{ "enabled",  NULL },
		{ NULL,       NULL },
	},
	"disabled"
};
static struct retro_core_option_v2_definition var_fbneo_debug_dip_1_7 = {
	"fbneo-debug-dip-1-7",
	"Debug Dip 1_7",
	NULL,
	"Enable Debug Dip 1_7",
	NULL,
	"neogeo",
	{
		{ "disabled", NULL },
		{ "enabled",  NULL },
		{ NULL,       NULL },
	},
	"disabled"
};
static struct retro_core_option_v2_definition var_fbneo_debug_dip_1_8 = {
	"fbneo-debug-dip-1-8",
	"Debug Dip 1_8",
	NULL,
	"Enable Debug Dip 1_8",
	NULL,
	"neogeo",
	{
		{ "disabled", NULL },
		{ "enabled",  NULL },
		{ NULL,       NULL },
	},
	"disabled"
};
static struct retro_core_option_v2_definition var_fbneo_debug_dip_2_1 = {
	"fbneo-debug-dip-2-1",
	"Debug Dip 2_1",
	NULL,
	"Enable Debug Dip 2_1",
	NULL,
	"neogeo",
	{
		{ "disabled", NULL },
		{ "enabled",  NULL },
		{ NULL,       NULL },
	},
	"disabled"
};
static struct retro_core_option_v2_definition var_fbneo_debug_dip_2_2 = {
	"fbneo-debug-dip-2-2",
	"Debug Dip 2_2",
	NULL,
	"Enable Debug Dip 2_2",
	NULL,
	"neogeo",
	{
		{ "disabled", NULL },
		{ "enabled",  NULL },
		{ NULL,       NULL },
	},
	"disabled"
};
static struct retro_core_option_v2_definition var_fbneo_debug_dip_2_3 = {
	"fbneo-debug-dip-2-3",
	"Debug Dip 2_3",
	NULL,
	"Enable Debug Dip 2_3",
	NULL,
	"neogeo",
	{
		{ "disabled", NULL },
		{ "enabled",  NULL },
		{ NULL,       NULL },
	},
	"disabled"
};
static struct retro_core_option_v2_definition var_fbneo_debug_dip_2_4 = {
	"fbneo-debug-dip-2-4",
	"Debug Dip 2_4",
	NULL,
	"Enable Debug Dip 2_4",
	NULL,
	"neogeo",
	{
		{ "disabled", NULL },
		{ "enabled",  NULL },
		{ NULL,       NULL },
	},
	"disabled"
};
static struct retro_core_option_v2_definition var_fbneo_debug_dip_2_5 = {
	"fbneo-debug-dip-2-5",
	"Debug Dip 2_5",
	NULL,
	"Enable Debug Dip 2_5",
	NULL,
	"neogeo",
	{
		{ "disabled", NULL },
		{ "enabled",  NULL },
		{ NULL,       NULL },
	},
	"disabled"
};
static struct retro_core_option_v2_definition var_fbneo_debug_dip_2_6 = {
	"fbneo-debug-dip-2-6",
	"Debug Dip 2_6",
	NULL,
	"Enable Debug Dip 2_6",
	NULL,
	"neogeo",
	{
		{ "disabled", NULL },
		{ "enabled",  NULL },
		{ NULL,       NULL },
	},
	"disabled"
};
static struct retro_core_option_v2_definition var_fbneo_debug_dip_2_7 = {
	"fbneo-debug-dip-2-7",
	"Debug Dip 2_7",
	NULL,
	"Enable Debug Dip 2_7",
	NULL,
	"neogeo",
	{
		{ "disabled", NULL },
		{ "enabled",  NULL },
		{ NULL,       NULL },
	},
	"disabled"
};
static struct retro_core_option_v2_definition var_fbneo_debug_dip_2_8 = {
	"fbneo-debug-dip-2-8",
	"Debug Dip 2_8",
	NULL,
	"Enable Debug Dip 2_8",
	NULL,
	"neogeo",
	{
		{ "disabled", NULL },
		{ "enabled",  NULL },
		{ NULL,       NULL },
	},
	"disabled"
};

#ifdef FBNEO_DEBUG
static struct retro_core_option_v2_definition var_fbneo_debug_layer_1 = {
	"fbneo-debug-layer-1",
	"Layer 1",
	NULL,
	"For debugging",
	NULL,
	"debug",
	{
		{ "disabled", NULL },
		{ "enabled",  NULL },
		{ NULL,       NULL },
	},
	"enabled"
};
static struct retro_core_option_v2_definition var_fbneo_debug_layer_2 = {
	"fbneo-debug-layer-2",
	"Layer 2",
	NULL,
	"For debugging",
	NULL,
	"debug",
	{
		{ "disabled", NULL },
		{ "enabled",  NULL },
		{ NULL,       NULL },
	},
	"enabled"
};
static struct retro_core_option_v2_definition var_fbneo_debug_layer_3 = {
	"fbneo-debug-layer-3",
	"Layer 3",
	NULL,
	"For debugging",
	NULL,
	"debug",
	{
		{ "disabled", NULL },
		{ "enabled",  NULL },
		{ NULL,       NULL },
	},
	"enabled"
};
static struct retro_core_option_v2_definition var_fbneo_debug_layer_4 = {
	"fbneo-debug-layer-4",
	"Layer 4",
	NULL,
	"For debugging",
	NULL,
	"debug",
	{
		{ "disabled", NULL },
		{ "enabled",  NULL },
		{ NULL,       NULL },
	},
	"enabled"
};
static struct retro_core_option_v2_definition var_fbneo_debug_sprite_1 = {
	"fbneo-debug-sprite-1",
	"Sprite 1",
	NULL,
	"For debugging",
	NULL,
	"debug",
	{
		{ "disabled", NULL },
		{ "enabled",  NULL },
		{ NULL,       NULL },
	},
	"enabled"
};
static struct retro_core_option_v2_definition var_fbneo_debug_sprite_2 = {
	"fbneo-debug-sprite-2",
	"Sprite 2",
	NULL,
	"For debugging",
	NULL,
	"debug",
	{
		{ "disabled", NULL },
		{ "enabled",  NULL },
		{ NULL,       NULL },
	},
	"enabled"
};
static struct retro_core_option_v2_definition var_fbneo_debug_sprite_3 = {
	"fbneo-debug-sprite-3",
	"Sprite 3",
	NULL,
	"For debugging",
	NULL,
	"debug",
	{
		{ "disabled", NULL },
		{ "enabled",  NULL },
		{ NULL,       NULL },
	},
	"enabled"
};
static struct retro_core_option_v2_definition var_fbneo_debug_sprite_4 = {
	"fbneo-debug-sprite-4",
	"Sprite 4",
	NULL,
	"For debugging",
	NULL,
	"debug",
	{
		{ "disabled", NULL },
		{ "enabled",  NULL },
		{ NULL,       NULL },
	},
	"enabled"
};
static struct retro_core_option_v2_definition var_fbneo_debug_sprite_5 = {
	"fbneo-debug-sprite-5",
	"Sprite 5",
	NULL,
	"For debugging",
	NULL,
	"debug",
	{
		{ "disabled", NULL },
		{ "enabled",  NULL },
		{ NULL,       NULL },
	},
	"enabled"
};
static struct retro_core_option_v2_definition var_fbneo_debug_sprite_6 = {
	"fbneo-debug-sprite-6",
	"Sprite 6",
	NULL,
	"For debugging",
	NULL,
	"debug",
	{
		{ "disabled", NULL },
		{ "enabled",  NULL },
		{ NULL,       NULL },
	},
	"enabled"
};
static struct retro_core_option_v2_definition var_fbneo_debug_sprite_7 = {
	"fbneo-debug-sprite-7",
	"Sprite 7",
	NULL,
	"For debugging",
	NULL,
	"debug",
	{
		{ "disabled", NULL },
		{ "enabled",  NULL },
		{ NULL,       NULL },
	},
	"enabled"
};
static struct retro_core_option_v2_definition var_fbneo_debug_sprite_8 = {
	"fbneo-debug-sprite-8",
	"Sprite 8",
	NULL,
	"For debugging",
	NULL,
	"debug",
	{
		{ "disabled", NULL },
		{ "enabled",  NULL },
		{ NULL,       NULL },
	},
	"enabled"
};
#endif

// Replace the char c_find by the char c_replace in the destination c string
char* str_char_replace(char* destination, char c_find, char c_replace)
{
	for (unsigned str_idx = 0; str_idx < strlen(destination); str_idx++)
	{
		if (destination[str_idx] == c_find)
			destination[str_idx] = c_replace;
	}

	return destination;
}

void set_neogeo_bios_availability(char *szName, uint32_t crc, bool ignoreCrc)
{
	for (int i = 0; neogeo_bioses[i].filename != NULL; i++)
	{
		if ((strcmp(neogeo_bioses[i].filename, szName) == 0 && ignoreCrc) || neogeo_bioses[i].crc == crc)
		{
			neogeo_bioses[i].available = 1;
			return;
		}
	}
}

static RomBiosInfo* find_neogeo_bios(uint32_t categories)
{
	for (int i = 0; neogeo_bioses[i].filename != NULL; i++)
	{
		if (neogeo_bioses[i].categories == categories && neogeo_bioses[i].available == 1)
		{
			return &neogeo_bioses[i];
		}
	}

	return NULL;
}

void set_neo_system_bios()
{
#ifndef NO_NEOGEO
	if (g_opt_neo_geo_mode == 0)
	{
		// Nothing to do in DIPSWITCH mode because the NeoSystem variable is changed by the DIP Switch core option
		log_cb(RETRO_LOG_INFO, "DIPSWITCH Neo Geo Mode selected => NeoSystem: 0x%02x.\n", NeoSystem);
	}
	else
	{
		RomBiosInfo *available_neogeo_bios = find_neogeo_bios(g_opt_neo_geo_mode);
		NeoSystem &= ~(UINT8)0x1f;
		if (available_neogeo_bios)
		{
			NeoSystem |= available_neogeo_bios->NeoSystem;
			log_cb(RETRO_LOG_INFO, "Found this bios for requested mode => NeoSystem: 0x%02x (%s [0x%08x] (%s)).\n", NeoSystem, available_neogeo_bios->filename, available_neogeo_bios->crc, available_neogeo_bios->friendly_name);
		}
		else
		{
			log_cb(RETRO_LOG_INFO, "No bios found for requested mode, falling back to DIPSWITCH => NeoSystem: 0x%02x.\n", NeoSystem);
		}
	}
#endif
}

void evaluate_neogeo_bios_mode(const char* drvname)
{
	if (!bIsNeogeoCartGame)
		return;

	bool is_bios_dipswitch_found = false;

	// search the BIOS dipswitch
	for (int dip_idx = 0; dip_idx < dipswitch_core_options.size(); dip_idx++)
	{
		if (dipswitch_core_options[dip_idx].friendly_name.compare("[Dipswitch] BIOS") == 0)
		{
			is_bios_dipswitch_found = true;
			if (dipswitch_core_options[dip_idx].values.size() > 0)
			{
				// if the default is different than 0, this means that a different Bios is needed
				if (dipswitch_core_options[dip_idx].default_bdi.nSetting != 0x00)
				{
					neogeo_use_specific_default_bios = true;
					break;
				}
			}
		}
	}

	// Games without the BIOS dipswitch don't handle alternative bioses very well
	if (!is_bios_dipswitch_found)
	{
		neogeo_use_specific_default_bios = true;
	}

	if (neogeo_use_specific_default_bios)
	{
		// disable the NeoGeo mode core option
		allow_neogeo_mode = false;

		// set the NeoGeo mode to DIPSWITCH to rely on the Default Bios Dipswitch
		g_opt_neo_geo_mode = 0;
	}
}

void set_environment()
{
	std::vector<const retro_core_option_v2_definition*> vars_systems;
	struct retro_core_option_v2_definition *option_defs_us;
#ifdef _MSC_VER
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
	#ifndef FORCE_USE_VFS
	#define FORCE_USE_VFS
	#endif
#endif
#endif

#ifdef FORCE_USE_VFS
	struct retro_vfs_interface_info vfs_iface_info;
#endif

	// Add the Global core options
	var_fbneo_allow_depth_32.desc                          = RETRO_DEPTH32_CAT_DESC;
	var_fbneo_allow_depth_32.info                          = RETRO_DEPTH32_CAT_INFO;
	vars_systems.push_back(&var_fbneo_allow_depth_32);

	var_fbneo_vertical_mode.desc                           = RETRO_VERTICAL_CAT_DESC;
	var_fbneo_vertical_mode.info                           = RETRO_VERTICAL_CAT_INFO;
	var_fbneo_vertical_mode.values[2].value                = RETRO_VERTICAL_VALUE_2;
	var_fbneo_vertical_mode.values[4].value                = RETRO_VERTICAL_VALUE_4;
	vars_systems.push_back(&var_fbneo_vertical_mode);

	var_fbneo_force_60hz.desc                              = RETRO_FORCE60_CAT_DESC;
	var_fbneo_force_60hz.info                              = RETRO_FORCE60_CAT_INFO;
	vars_systems.push_back(&var_fbneo_force_60hz);

	var_fbneo_allow_patched_romsets.desc                   = RETRO_PATCHED_CAT_DESC;
	var_fbneo_allow_patched_romsets.info                   = RETRO_PATCHED_CAT_INFO;
	vars_systems.push_back(&var_fbneo_allow_patched_romsets);

	var_fbneo_allow_ignore_crc.desc                        = RETRO_IGNORE_CRC_DESC;
	var_fbneo_allow_ignore_crc.info                        = RETRO_IGNORE_CRC_INFO;
	vars_systems.push_back(&var_fbneo_allow_ignore_crc);

	var_fbneo_analog_speed.desc                            = RETRO_ANALOG_CAT_DESC;
	var_fbneo_analog_speed.info                            = RETRO_ANALOG_CAT_INFO;
	vars_systems.push_back(&var_fbneo_analog_speed);

	var_fbneo_socd.desc                                    = RETRO_SOCD_DESC;
	var_fbneo_socd.info                                    = RETRO_SOCD_INFO;
	var_fbneo_socd.values[1].label                         = RETRO_SOCD_LABEL_1;
	var_fbneo_socd.values[2].label                         = RETRO_SOCD_LABEL_2;
	var_fbneo_socd.values[3].label                         = RETRO_SOCD_LABEL_3;
	var_fbneo_socd.values[4].label                         = RETRO_SOCD_LABEL_4;
	var_fbneo_socd.values[5].label                         = RETRO_SOCD_LABEL_5;
	var_fbneo_socd.values[6].label                         = RETRO_SOCD_LABEL_6;
	vars_systems.push_back(&var_fbneo_socd);

	var_fbneo_lightgun_crosshair_emulation.desc            = RETRO_CROSSHAIR_CAT_DESC;
	var_fbneo_lightgun_crosshair_emulation.info            = RETRO_CROSSHAIR_CAT_INFO;
	var_fbneo_lightgun_crosshair_emulation.values[0].value = RETRO_CROSSHAIR_VALUE_0;
	var_fbneo_lightgun_crosshair_emulation.values[1].value = RETRO_CROSSHAIR_VALUE_1;
	var_fbneo_lightgun_crosshair_emulation.values[2].value = RETRO_CROSSHAIR_VALUE_2;
	var_fbneo_lightgun_crosshair_emulation.default_value   = RETRO_CROSSHAIR_VALUE_0;
	vars_systems.push_back(&var_fbneo_lightgun_crosshair_emulation);

	var_fbneo_cpu_speed_adjust.desc                        = RETRO_CPUSPEED_CAT_DESC;
	var_fbneo_cpu_speed_adjust.info                        = RETRO_CPUSPEED_CAT_INFO;
	vars_systems.push_back(&var_fbneo_cpu_speed_adjust);

#ifdef USE_CYCLONE
	var_fbneo_cyclone.desc = RETRO_CYCLONE_CAT_DESC;
	var_fbneo_cyclone.info = RETRO_CYCLONE_CAT_INFO;
	vars_systems.push_back(&var_fbneo_cyclone);
#endif
	if (BurnDrvGetFlags() & BDF_HISCORE_SUPPORTED)
	{
		var_fbneo_hiscores.desc = RETRO_HISCORES_CAT_DESC;
		var_fbneo_hiscores.info = RETRO_HISCORES_CAT_INFO;
		vars_systems.push_back(&var_fbneo_hiscores);
	}

	if (pgi_diag)
	{
		var_fbneo_diagnostic_input.desc             = RETRO_DIAGNOSTIC_CAT_DESC;
		var_fbneo_diagnostic_input.info             = RETRO_DIAGNOSTIC_CAT_INFO;
		var_fbneo_diagnostic_input.values[ 0].value = RETRO_DIAGNOSTIC_VALUE_0;
		var_fbneo_diagnostic_input.values[ 1].value = RETRO_DIAGNOSTIC_VALUE_1;
		var_fbneo_diagnostic_input.values[ 3].value = RETRO_DIAGNOSTIC_VALUE_3;
		var_fbneo_diagnostic_input.values[ 5].value = RETRO_DIAGNOSTIC_VALUE_5;
		var_fbneo_diagnostic_input.values[ 6].value = RETRO_DIAGNOSTIC_VALUE_6;
		var_fbneo_diagnostic_input.values[ 8].value = RETRO_DIAGNOSTIC_VALUE_8;
		var_fbneo_diagnostic_input.values[10].value = RETRO_DIAGNOSTIC_VALUE_10;
		var_fbneo_diagnostic_input.default_value    = RETRO_DIAGNOSTIC_VALUE_1;
		vars_systems.push_back(&var_fbneo_diagnostic_input);
	}

	if (bIsNeogeoCartGame)
	{
		// Add the Neo Geo core options
		if (allow_neogeo_mode)
		{
			var_fbneo_neogeo_mode.desc            = RETRO_NGMODE_DEF_DESC;
			var_fbneo_neogeo_mode.info            = RETRO_NGMODE_DEF_INFO;
			var_fbneo_neogeo_mode.values[0].label = RETRO_NGMODE_LABEL_0;
			vars_systems.push_back(&var_fbneo_neogeo_mode);
		}

		var_fbneo_memcard_mode.desc            = RETRO_MEMCARD_DEF_DESC;
		var_fbneo_memcard_mode.info            = RETRO_MEMCARD_DEF_INFO;
		var_fbneo_memcard_mode.values[1].value = RETRO_MEMCARD_VALUE_1;
		var_fbneo_memcard_mode.values[2].value = RETRO_MEMCARD_VALUE_2;
		vars_systems.push_back(&var_fbneo_memcard_mode);

		if (pgi_debug_dip_1)
		{
			var_fbneo_debug_dip_1_1.desc = RETRO_DEBUG11_DEF_DESC;
			var_fbneo_debug_dip_1_1.info = RETRO_DEBUG11_DEF_INFO;
			vars_systems.push_back(&var_fbneo_debug_dip_1_1);

			var_fbneo_debug_dip_1_2.desc = RETRO_DEBUG12_DEF_DESC;
			var_fbneo_debug_dip_1_2.info = RETRO_DEBUG12_DEF_INFO;
			vars_systems.push_back(&var_fbneo_debug_dip_1_2);

			var_fbneo_debug_dip_1_3.desc = RETRO_DEBUG13_DEF_DESC;
			var_fbneo_debug_dip_1_3.info = RETRO_DEBUG13_DEF_INFO;
			vars_systems.push_back(&var_fbneo_debug_dip_1_3);

			var_fbneo_debug_dip_1_4.desc = RETRO_DEBUG14_DEF_DESC;
			var_fbneo_debug_dip_1_4.info = RETRO_DEBUG14_DEF_INFO;
			vars_systems.push_back(&var_fbneo_debug_dip_1_4);

			var_fbneo_debug_dip_1_5.desc = RETRO_DEBUG15_DEF_DESC;
			var_fbneo_debug_dip_1_5.info = RETRO_DEBUG15_DEF_INFO;
			vars_systems.push_back(&var_fbneo_debug_dip_1_5);

			var_fbneo_debug_dip_1_6.desc = RETRO_DEBUG16_DEF_DESC;
			var_fbneo_debug_dip_1_6.info = RETRO_DEBUG16_DEF_INFO;
			vars_systems.push_back(&var_fbneo_debug_dip_1_6);

			var_fbneo_debug_dip_1_7.desc = RETRO_DEBUG17_DEF_DESC;
			var_fbneo_debug_dip_1_7.info = RETRO_DEBUG17_DEF_INFO;
			vars_systems.push_back(&var_fbneo_debug_dip_1_7);

			var_fbneo_debug_dip_1_8.desc = RETRO_DEBUG18_DEF_DESC;
			var_fbneo_debug_dip_1_8.info = RETRO_DEBUG18_DEF_INFO;
			vars_systems.push_back(&var_fbneo_debug_dip_1_8);
		}
		if (pgi_debug_dip_2)
		{
			var_fbneo_debug_dip_2_1.desc = RETRO_DEBUG21_DEF_DESC;
			var_fbneo_debug_dip_2_1.info = RETRO_DEBUG21_DEF_INFO;
			vars_systems.push_back(&var_fbneo_debug_dip_2_1);

			var_fbneo_debug_dip_2_2.desc = RETRO_DEBUG22_DEF_DESC;
			var_fbneo_debug_dip_2_2.info = RETRO_DEBUG22_DEF_INFO;
			vars_systems.push_back(&var_fbneo_debug_dip_2_2);

			var_fbneo_debug_dip_2_3.desc = RETRO_DEBUG23_DEF_DESC;
			var_fbneo_debug_dip_2_3.info = RETRO_DEBUG23_DEF_INFO;
			vars_systems.push_back(&var_fbneo_debug_dip_2_3);

			var_fbneo_debug_dip_2_4.desc = RETRO_DEBUG24_DEF_DESC;
			var_fbneo_debug_dip_2_4.info = RETRO_DEBUG24_DEF_INFO;
			vars_systems.push_back(&var_fbneo_debug_dip_2_4);

			var_fbneo_debug_dip_2_5.desc = RETRO_DEBUG25_DEF_DESC;
			var_fbneo_debug_dip_2_5.info = RETRO_DEBUG25_DEF_INFO;
			vars_systems.push_back(&var_fbneo_debug_dip_2_5);

			var_fbneo_debug_dip_2_6.desc = RETRO_DEBUG26_DEF_DESC;
			var_fbneo_debug_dip_2_6.info = RETRO_DEBUG26_DEF_INFO;
			vars_systems.push_back(&var_fbneo_debug_dip_2_6);

			var_fbneo_debug_dip_2_7.desc = RETRO_DEBUG27_DEF_DESC;
			var_fbneo_debug_dip_2_7.info = RETRO_DEBUG27_DEF_INFO;
			vars_systems.push_back(&var_fbneo_debug_dip_2_7);

			var_fbneo_debug_dip_2_8.desc = RETRO_DEBUG28_DEF_DESC;
			var_fbneo_debug_dip_2_8.info = RETRO_DEBUG28_DEF_INFO;
			vars_systems.push_back(&var_fbneo_debug_dip_2_8);
		}
	}

	// Frameskip settings
	if (bLibretroSupportsAudioBuffStatus)
	{
		var_fbneo_frameskip_type.desc             = RETRO_FRAMESKIP_DEF_DESC;
		var_fbneo_frameskip_type.info             = RETRO_FRAMESKIP_DEF_INFO;
		var_fbneo_frameskip_type.values[1].value  = RETRO_FRAMESKIP_VALUE_1;
		var_fbneo_frameskip_type.values[2].value  = RETRO_FRAMESKIP_VALUE_2;
		var_fbneo_frameskip_type.values[3].value  = RETRO_FRAMESKIP_VALUE_3;
		vars_systems.push_back(&var_fbneo_frameskip_type);

		var_fbneo_frameskip_manual_threshold.desc = RETRO_FSTHRESHOLD_DEF_DESC;
		var_fbneo_frameskip_manual_threshold.info = RETRO_FSTHRESHOLD_DEF_INFO;
		vars_systems.push_back(&var_fbneo_frameskip_manual_threshold);
	}

	var_fbneo_fixed_frameskip.desc            = RETRO_FSFIXED_DEF_DESC;
	var_fbneo_fixed_frameskip.info            = RETRO_FSFIXED_DEF_INFO;
	var_fbneo_fixed_frameskip.values[0].label = RETRO_FSFIXED_LABEL_0;
	var_fbneo_fixed_frameskip.values[1].label = RETRO_FSFIXED_LABEL_1;
	var_fbneo_fixed_frameskip.values[2].label = RETRO_FSFIXED_LABEL_2;
	var_fbneo_fixed_frameskip.values[3].label = RETRO_FSFIXED_LABEL_3;
	var_fbneo_fixed_frameskip.values[4].label = RETRO_FSFIXED_LABEL_4;
	var_fbneo_fixed_frameskip.values[5].label = RETRO_FSFIXED_LABEL_5;
	vars_systems.push_back(&var_fbneo_fixed_frameskip);

	// Audio settings
	if (nGameType != RETRO_GAME_TYPE_NEOCD)
	{
		var_fbneo_samplerate.desc = RETRO_SAMPLERATE_DEF_DESC;
		var_fbneo_samplerate.info = RETRO_SAMPLERATE_DEF_INFO;
		vars_systems.push_back(&var_fbneo_samplerate);
	}
	var_fbneo_sample_interpolation.desc            = RETRO_SAMPLE_INTERPOLATION_DEF_DESC;
	var_fbneo_sample_interpolation.info            = RETRO_SAMPLE_INTERPOLATION_DEF_INFO;
	var_fbneo_sample_interpolation.values[1].value = RETRO_SAMPLE_INTERPOLATION_VALUE_1;
	var_fbneo_sample_interpolation.values[2].value = RETRO_SAMPLE_INTERPOLATION_VALUE_2;
	var_fbneo_sample_interpolation.default_value   = RETRO_SAMPLE_INTERPOLATION_VALUE_2;
	vars_systems.push_back(&var_fbneo_sample_interpolation);

	var_fbneo_fm_interpolation.desc                = RETRO_FM_INTERPOLATION_DEF_DESC;
	var_fbneo_fm_interpolation.info                = RETRO_FM_INTERPOLATION_DEF_INFO;
	var_fbneo_fm_interpolation.values[1].value     = RETRO_FM_INTERPOLATION_VALUE_1;
	var_fbneo_fm_interpolation.default_value       = RETRO_FM_INTERPOLATION_VALUE_1;
	vars_systems.push_back(&var_fbneo_fm_interpolation);

	var_fbneo_lowpass_filter.desc                  = RETRO_LOWPASS_FILTER_DEF_DESC;
	var_fbneo_lowpass_filter.info                  = RETRO_LOWPASS_FILTER_DEF_INFO;
	vars_systems.push_back(&var_fbneo_lowpass_filter);

#ifdef FBNEO_DEBUG
	// Debug settings
	var_fbneo_debug_layer_1.desc  = RETRO_DEBUG_LAYER_1_DEF_DESC;
	var_fbneo_debug_layer_1.info  = RETRO_DEBUG_DEF_INFO;
	vars_systems.push_back(&var_fbneo_debug_layer_1);

	var_fbneo_debug_layer_2.desc  = RETRO_DEBUG_LAYER_2_DEF_DESC;
	var_fbneo_debug_layer_2.info  = RETRO_DEBUG_DEF_INFO;
	vars_systems.push_back(&var_fbneo_debug_layer_2);

	var_fbneo_debug_layer_3.desc  = RETRO_DEBUG_LAYER_3_DEF_DESC;
	var_fbneo_debug_layer_3.info  = RETRO_DEBUG_DEF_INFO;
	vars_systems.push_back(&var_fbneo_debug_layer_3);

	var_fbneo_debug_layer_4.desc  = RETRO_DEBUG_LAYER_4_DEF_DESC;
	var_fbneo_debug_layer_4.info  = RETRO_DEBUG_DEF_INFO;
	vars_systems.push_back(&var_fbneo_debug_layer_4);

	var_fbneo_debug_sprite_1.desc = RETRO_DEBUG_SPRITE_1_DEF_DESC;
	var_fbneo_debug_sprite_1.info = RETRO_DEBUG_DEF_INFO;
	vars_systems.push_back(&var_fbneo_debug_sprite_1);

	var_fbneo_debug_sprite_2.desc = RETRO_DEBUG_SPRITE_2_DEF_DESC;
	var_fbneo_debug_sprite_2.info = RETRO_DEBUG_DEF_INFO;
	vars_systems.push_back(&var_fbneo_debug_sprite_2);

	var_fbneo_debug_sprite_3.desc = RETRO_DEBUG_SPRITE_3_DEF_DESC;
	var_fbneo_debug_sprite_3.info = RETRO_DEBUG_DEF_INFO;
	vars_systems.push_back(&var_fbneo_debug_sprite_3);

	var_fbneo_debug_sprite_4.desc = RETRO_DEBUG_SPRITE_4_DEF_DESC;
	var_fbneo_debug_sprite_4.info = RETRO_DEBUG_DEF_INFO;
	vars_systems.push_back(&var_fbneo_debug_sprite_4);

	var_fbneo_debug_sprite_5.desc = RETRO_DEBUG_SPRITE_5_DEF_DESC;
	var_fbneo_debug_sprite_5.info = RETRO_DEBUG_DEF_INFO;
	vars_systems.push_back(&var_fbneo_debug_sprite_5);

	var_fbneo_debug_sprite_6.desc = RETRO_DEBUG_SPRITE_6_DEF_DESC;
	var_fbneo_debug_sprite_6.info = RETRO_DEBUG_DEF_INFO;
	vars_systems.push_back(&var_fbneo_debug_sprite_6);

	var_fbneo_debug_sprite_7.desc = RETRO_DEBUG_SPRITE_7_DEF_DESC;
	var_fbneo_debug_sprite_7.info = RETRO_DEBUG_DEF_INFO;
	vars_systems.push_back(&var_fbneo_debug_sprite_7);

	var_fbneo_debug_sprite_8.desc = RETRO_DEBUG_SPRITE_8_DEF_DESC;
	var_fbneo_debug_sprite_8.info = RETRO_DEBUG_DEF_INFO;
	vars_systems.push_back(&var_fbneo_debug_sprite_8);
#endif

	int nbr_vars     = vars_systems.size();
	int nbr_dips     = dipswitch_core_options.size();
	int nbr_cheats   = cheat_core_options.size();
	int nbr_ipses    = ips_core_options.size();
	int nbr_romdatas = romdata_core_options.size();
	int nbr_command_dat = get_command_dat_count();

#if 0
	log_cb(RETRO_LOG_INFO, "set_environment: SYSTEM: %d, DIPSWITCH: %d\n", nbr_vars, nbr_dips);
#endif

	option_defs_us = (struct retro_core_option_v2_definition*)calloc(nbr_vars + nbr_dips + nbr_cheats + nbr_ipses + nbr_romdatas + nbr_command_dat + 1, sizeof(struct retro_core_option_v2_definition));

	int idx_var = 0;

	// Add the System core options
	for (int i = 0; i < nbr_vars; i++, idx_var++)
	{
		option_defs_us[idx_var] = *vars_systems[i];
	}

	// Add the DIP switches core options
	for (int dip_idx = 0; dip_idx < nbr_dips; dip_idx++)
	{
		option_defs_us[idx_var].category_key     = "dipswitch";
		option_defs_us[idx_var].key              = dipswitch_core_options[dip_idx].option_name.c_str();
		option_defs_us[idx_var].desc             = dipswitch_core_options[dip_idx].friendly_name.c_str();
		option_defs_us[idx_var].desc_categorized = dipswitch_core_options[dip_idx].friendly_name_categorized.c_str();
		option_defs_us[idx_var].default_value    = dipswitch_core_options[dip_idx].default_bdi.szText;
		// Instead of filtering out the dips, make the description a warning if it's a neogeo game using a different default bios
		if (neogeo_use_specific_default_bios && bIsNeogeoCartGame && dipswitch_core_options[dip_idx].friendly_name.compare("[Dipswitch] BIOS") == 0)
			option_defs_us[idx_var].info         = RETRO_NGBIOS_DEF_INFO_0;
		else
			option_defs_us[idx_var].info         = RETRO_NGBIOS_DEF_INFO_1;
		for (int dip_value_idx = 0; dip_value_idx < dipswitch_core_options[dip_idx].values.size(); dip_value_idx++)
		{
			option_defs_us[idx_var].values[dip_value_idx].value = dipswitch_core_options[dip_idx].values[dip_value_idx].friendly_name.c_str();
		}
		option_defs_us[idx_var].values[dipswitch_core_options[dip_idx].values.size()].value = NULL;
		if (option_defs_us[idx_var].default_value == NULL) HandleMessage(RETRO_LOG_ERROR, "Default value for '%s' not found, it's a bug, please report it\n", option_defs_us[idx_var].desc);
		idx_var++;
	}

	// Add the cheats core options
	for (int cheat_idx = 0; cheat_idx < nbr_cheats; cheat_idx++)
	{
		option_defs_us[idx_var].category_key     = "cheat";
		option_defs_us[idx_var].key              = cheat_core_options[cheat_idx].option_name.c_str();
		option_defs_us[idx_var].desc             = cheat_core_options[cheat_idx].friendly_name.c_str();
		option_defs_us[idx_var].desc_categorized = cheat_core_options[cheat_idx].friendly_name_categorized.c_str();
		option_defs_us[idx_var].default_value    = cheat_core_options[cheat_idx].default_value.c_str();
		option_defs_us[idx_var].info             = RETRO_CHEAT_DEF_INFO;
		for (int cheat_value_idx = 0; cheat_value_idx < cheat_core_options[cheat_idx].values.size(); cheat_value_idx++)
		{
			option_defs_us[idx_var].values[cheat_value_idx].value = cheat_core_options[cheat_idx].values[cheat_value_idx].friendly_name.c_str();
		}
		option_defs_us[idx_var].values[cheat_core_options[cheat_idx].values.size()].value = NULL;
		idx_var++;
	}

	// Add the ipses core options
	for (int ips_idx = 0; ips_idx < nbr_ipses; ips_idx++)
	{
		option_defs_us[idx_var].key              = ips_core_options[ips_idx].option_name.c_str();
		option_defs_us[idx_var].desc             = ips_core_options[ips_idx].friendly_name.c_str();
		option_defs_us[idx_var].info             = ips_core_options[ips_idx].friendly_name_categorized.c_str();
		option_defs_us[idx_var].category_key     = "ips";
		option_defs_us[idx_var].values[0].value  = "disabled";
		option_defs_us[idx_var].values[1].value  = "enabled";
		option_defs_us[idx_var].values[2].value  = NULL;
		option_defs_us[idx_var].default_value    = "disabled";
		idx_var++;
	}

	// Add the romdatas core options
	for (int romdata_idx = 0; romdata_idx < nbr_romdatas; romdata_idx++)
	{
		option_defs_us[idx_var].key             = romdata_core_options[romdata_idx].option_name.c_str();
		option_defs_us[idx_var].desc            = romdata_core_options[romdata_idx].friendly_name.c_str();
		option_defs_us[idx_var].info            = RETRO_ROMDATA_DEF_INFO;
		option_defs_us[idx_var].category_key    = "romdata";
		option_defs_us[idx_var].values[0].value = "disabled";
		option_defs_us[idx_var].values[1].value = "enabled";
		option_defs_us[idx_var].values[2].value = NULL;
		option_defs_us[idx_var].default_value   = "disabled";
		idx_var++;
	}

	idx_var = AddCommandDatOptions(idx_var,option_defs_us);

	option_defs_us[idx_var] = var_empty;

	static struct retro_core_option_v2_category option_cats_us[] =
	{
		{
			"neogeo",
			RETRO_NEOGEO_CAT_DESC,
			RETRO_NEOGEO_CAT_INFO
		},
		{
			"frameskip",
			RETRO_FRAME_CAT_DESC,
			RETRO_FRAME_CAT_INFO
		},
		{
			"audio",
			RETRO_AUDIO_CAT_DESC,
			RETRO_AUDIO_CAT_INFO
		},
		{
			"dipswitch",
			RETRO_DIP_CAT_DESC,
			RETRO_DIP_CAT_INFO
		},
		{
			"cheat",
			RETRO_CHEAT_CAT_DESC,
			RETRO_CHEAT_CAT_INFO
		},
		{
			"ips",
			RETRO_IPS_CAT_DESC,
			RETRO_IPS_CAT_INFO
		},
		{
			"romdata",
			"RomData",
			RETRO_ROMDATA_CAT_INFO
		},
		{
			"command_dat",
			RETRO_COMMAND_DESC,
			RETRO_COMMAND_INFO
		},
#ifdef FBNEO_DEBUG
		{
			"debug",
			RETRO_DEBUG_CAT_DESC,
			RETRO_DEBUG_CAT_INFO
		},
#endif
		{ NULL, NULL, NULL },
	};

	struct retro_core_options_v2 options_us = {
		option_cats_us,
		option_defs_us
	};

	unsigned version = 0;

	if (!environ_cb(RETRO_ENVIRONMENT_GET_CORE_OPTIONS_VERSION, &version))
		version = 0;

	if (version >= 2)
	{
		environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_V2, &options_us);
	}
	else
	{
		size_t i, j;
		size_t num_options                                     = 0;
		struct retro_core_option_definition *option_v1_defs_us = NULL;
		struct retro_variable *variables                       = NULL;
		char **values_buf                                      = NULL;

		/* Determine total number of options */
		while (true)
		{
			if (option_defs_us[num_options].key)
				num_options++;
			else
				break;
		}

		if (version >= 1)
		{
			/* Allocate US array */
			option_v1_defs_us = (struct retro_core_option_definition *)calloc(num_options + 1, sizeof(struct retro_core_option_definition));

			/* Copy parameters from option_defs_us array */
			for (i = 0; i < num_options; i++)
			{
				struct retro_core_option_v2_definition *option_def_us = &option_defs_us[i];
				struct retro_core_option_value *option_values         = option_def_us->values;
				struct retro_core_option_definition *option_v1_def_us = &option_v1_defs_us[i];
				struct retro_core_option_value *option_v1_values      = option_v1_def_us->values;

				option_v1_def_us->key           = option_def_us->key;
				option_v1_def_us->desc          = option_def_us->desc;
				option_v1_def_us->info          = option_def_us->info;
				option_v1_def_us->default_value = option_def_us->default_value;

				/* Values must be copied individually... */
				while (option_values->value)
				{
					option_v1_values->value = option_values->value;
					option_v1_values->label = option_values->label;

					option_values++;
					option_v1_values++;
				}
			}

			environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS, option_v1_defs_us);
		}
		else
		{

			/* Allocate arrays */
			variables  = (struct retro_variable *)calloc(num_options + 1, sizeof(struct retro_variable));
			values_buf = (char **)calloc(num_options, sizeof(char *));

			if (!variables || !values_buf)
				goto error;

			/* Copy parameters from option_defs_us array */
			for (i = 0; i < num_options; i++)
			{
				const char *key                        = option_defs_us[i].key;
				const char *desc                       = option_defs_us[i].desc;
				const char *default_value              = option_defs_us[i].default_value;
				struct retro_core_option_value *values = option_defs_us[i].values;
				size_t buf_len                         = 3;
				size_t default_index                   = 0;

				values_buf[i] = NULL;

				if (desc)
				{
					size_t num_values = 0;

					/* Determine number of values */
					while (true)
					{
						if (values[num_values].value)
						{
							/* Check if this is the default value */
							if (default_value)
								if (strcmp(values[num_values].value, default_value) == 0)
									default_index = num_values;

							buf_len += strlen(values[num_values].value);
							num_values++;
						}
						else
							break;
					}

					if (num_values > 0)
					{

						buf_len += num_values - 1;
						buf_len += strlen(desc);

						values_buf[i] = (char *)calloc(buf_len, sizeof(char));
						if (!values_buf[i])
							goto error;

						strcpy(values_buf[i], desc);
						strcat(values_buf[i], "; ");

						/* Default value goes first */
						strcat(values_buf[i], values[default_index].value);

						/* Add remaining values */
						for (j = 0; j < num_values; j++)
						{
							if (j != default_index)
							{
								strcat(values_buf[i], "|");
								strcat(values_buf[i], values[j].value);
							}
						}
					}
				}

				variables[i].key   = key;
				variables[i].value = values_buf[i];
			}

			/* Set variables */
			environ_cb(RETRO_ENVIRONMENT_SET_VARIABLES, variables);

		}

error:
		/* Clean up */

		if (option_defs_us)
		{
			free(option_defs_us);
			option_defs_us = NULL;
		}

		if (option_v1_defs_us)
		{
			free(option_v1_defs_us);
			option_v1_defs_us = NULL;
		}

		if (values_buf)
		{
			for (i = 0; i < num_options; i++)
			{
				if (values_buf[i])
				{
					free(values_buf[i]);
					values_buf[i] = NULL;
				}
			}

			free(values_buf);
			values_buf = NULL;
		}

		if (variables)
		{
			free(variables);
			variables = NULL;
		}
	}

	// Initialize VFS
	// Only on UWP for now, since EEPROM saving is not VFS aware
#ifdef FORCE_USE_VFS
	vfs_iface_info.required_interface_version = FILESTREAM_REQUIRED_VFS_VERSION;
	vfs_iface_info.iface                      = NULL;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VFS_INTERFACE, &vfs_iface_info))
		filestream_vfs_init(&vfs_iface_info);
#endif
}

TCHAR* AdaptiveEncodingReads(const TCHAR* pszFileName)
{
	return NULL;
}

static int percent_parser(const char *value)
{
	INT32 nVal = atoi(value);
	if (nVal == 0)
		nVal = 100;

	return (int)((double)nVal * 256.0 / 100.0 + 0.5);
}

void check_variables(void)
{
	struct retro_variable var = {0};

	var.key = var_fbneo_cpu_speed_adjust.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		nBurnCPUSpeedAdjust = percent_parser(var.value);
	}

	var.key = var_fbneo_allow_depth_32.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (strcmp(var.value, "enabled") == 0)
			bAllowDepth32 = true;
		else
			bAllowDepth32 = false;
	}

	var.key = var_fbneo_vertical_mode.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (     strcmp(var.value, "enabled") == 0)
			nVerticalMode = 1;
		else if (strcmp(var.value, RETRO_VERTICAL_VALUE_2) == 0)
			nVerticalMode = 2;
		else if (strcmp(var.value, "TATE") == 0)
			nVerticalMode = 3;
		else if (strcmp(var.value, RETRO_VERTICAL_VALUE_4) == 0)
			nVerticalMode = 4;
		else
			nVerticalMode = 0;
	}

	var.key = var_fbneo_force_60hz.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (strcmp(var.value, "enabled") == 0)
		{
			bForce60Hz = true;
			float refresh_rate;
			if (environ_cb(RETRO_ENVIRONMENT_GET_TARGET_REFRESH_RATE, &refresh_rate)) {
				if (refresh_rate > 59.00 && refresh_rate < 61.00)
					dForcedFrameRate = (double)refresh_rate;
			}
		}
		else
			bForce60Hz = false;
	}

	if (bLibretroSupportsAudioBuffStatus)
	{
		var.key = var_fbneo_frameskip_type.key;
		if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
		{
			if (     strcmp(var.value, "disabled") == 0)
				nFrameskipType = 0;
			else if (strcmp(var.value, RETRO_FRAMESKIP_VALUE_1) == 0)
				nFrameskipType = 1;
			else if (strcmp(var.value, RETRO_FRAMESKIP_VALUE_2) == 0)
				nFrameskipType = 2;
			else if (strcmp(var.value, RETRO_FRAMESKIP_VALUE_3) == 0)
				nFrameskipType = 3;
		}

		var.key = var_fbneo_frameskip_manual_threshold.key;
		if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
			nFrameskipThreshold = strtol(var.value, NULL, 10);
	}

	var.key = var_fbneo_fixed_frameskip.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (     strcmp(var.value, "0") == 0)
			nFrameskip = 1;
		else if (strcmp(var.value, "1") == 0)
			nFrameskip = 2;
		else if (strcmp(var.value, "2") == 0)
			nFrameskip = 3;
		else if (strcmp(var.value, "3") == 0)
			nFrameskip = 4;
		else if (strcmp(var.value, "4") == 0)
			nFrameskip = 5;
		else if (strcmp(var.value, "5") == 0)
			nFrameskip = 6;
	}

	if (pgi_diag)
	{
		var.key = var_fbneo_diagnostic_input.key;
		if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
		{
			if (    strcmp(var.value, RETRO_DIAGNOSTIC_VALUE_1) == 0)
			{
				diag_input = diag_input_start;
				SetDiagInpHoldFrameDelay(60);
			}
			else if(strcmp(var.value, "Start + A + B") == 0)
			{
				diag_input = diag_input_start_a_b;
				SetDiagInpHoldFrameDelay(0);
			}
			else if(strcmp(var.value, RETRO_DIAGNOSTIC_VALUE_3) == 0)
			{
				diag_input = diag_input_start_a_b;
				SetDiagInpHoldFrameDelay(60);
			}
			else if(strcmp(var.value, "Start + L + R") == 0)
			{
				diag_input = diag_input_start_l_r;
				SetDiagInpHoldFrameDelay(0);
			}
			else if(strcmp(var.value, RETRO_DIAGNOSTIC_VALUE_5) == 0)
			{
				diag_input = diag_input_start_l_r;
				SetDiagInpHoldFrameDelay(60);
			}
			else if(strcmp(var.value, RETRO_DIAGNOSTIC_VALUE_6) == 0)
			{
				diag_input = diag_input_select;
				SetDiagInpHoldFrameDelay(60);
			}
			else if(strcmp(var.value, "Select + A + B") == 0)
			{
				diag_input = diag_input_select_a_b;
				SetDiagInpHoldFrameDelay(0);
			}
			else if(strcmp(var.value, RETRO_DIAGNOSTIC_VALUE_8) == 0)
			{
				diag_input = diag_input_select_a_b;
				SetDiagInpHoldFrameDelay(60);
			}
			else if(strcmp(var.value, "Select + L + R") == 0)
			{
				diag_input = diag_input_select_l_r;
				SetDiagInpHoldFrameDelay(0);
			}
			else if(strcmp(var.value, RETRO_DIAGNOSTIC_VALUE_10) == 0)
			{
				diag_input = diag_input_select_l_r;
				SetDiagInpHoldFrameDelay(60);
			}
			else
			{
				diag_input = NULL;
				SetDiagInpHoldFrameDelay(0);
			}
		}
	}

	if (bIsNeogeoCartGame)
	{
		if (allow_neogeo_mode)
		{
			var.key = var_fbneo_neogeo_mode.key;
			if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
			{
				if (     strcmp(var.value, "MVS_EUR") == 0)
					g_opt_neo_geo_mode = NEOGEO_MVS | NEOGEO_EUR;
				else if (strcmp(var.value, "MVS_USA") == 0)
					g_opt_neo_geo_mode = NEOGEO_MVS | NEOGEO_USA;
				else if (strcmp(var.value, "MVS_JAP") == 0)
					g_opt_neo_geo_mode = NEOGEO_MVS | NEOGEO_JAP;
				else if (strcmp(var.value, "AES_EUR") == 0)
					g_opt_neo_geo_mode = NEOGEO_AES | NEOGEO_EUR;
				else if (strcmp(var.value, "AES_JAP") == 0)
					g_opt_neo_geo_mode = NEOGEO_AES | NEOGEO_JAP;
				else if (strcmp(var.value, "UNIBIOS") == 0)
					g_opt_neo_geo_mode = NEOGEO_UNI;
				else
					g_opt_neo_geo_mode = 0;
			}
		}
		var.key = var_fbneo_memcard_mode.key;
		if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
		{
			if (     strcmp(var.value, "disabled") == 0)
				nMemcardMode = 0;
			else if (strcmp(var.value, RETRO_MEMCARD_VALUE_1) == 0)
				nMemcardMode = 1;
			else if (strcmp(var.value, RETRO_MEMCARD_VALUE_2) == 0)
				nMemcardMode = 2;
		}
		if (pgi_debug_dip_1)
		{
			var.key = var_fbneo_debug_dip_1_1.key;
			if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
			{
				if (strcmp(var.value, "enabled") == 0)
					pgi_debug_dip_1->Input.Constant.nConst |= 0x01;
				else
					pgi_debug_dip_1->Input.Constant.nConst &= ~0x01;
			}
			var.key = var_fbneo_debug_dip_1_2.key;
			if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
			{
				if (strcmp(var.value, "enabled") == 0)
					pgi_debug_dip_1->Input.Constant.nConst |= 0x02;
				else
					pgi_debug_dip_1->Input.Constant.nConst &= ~0x02;
			}
			var.key = var_fbneo_debug_dip_1_3.key;
			if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
			{
				if (strcmp(var.value, "enabled") == 0)
					pgi_debug_dip_1->Input.Constant.nConst |= 0x04;
				else
					pgi_debug_dip_1->Input.Constant.nConst &= ~0x04;
			}
			var.key = var_fbneo_debug_dip_1_4.key;
			if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
			{
				if (strcmp(var.value, "enabled") == 0)
					pgi_debug_dip_1->Input.Constant.nConst |= 0x08;
				else
					pgi_debug_dip_1->Input.Constant.nConst &= ~0x08;
			}
			var.key = var_fbneo_debug_dip_1_5.key;
			if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
			{
				if (strcmp(var.value, "enabled") == 0)
					pgi_debug_dip_1->Input.Constant.nConst |= 0x10;
				else
					pgi_debug_dip_1->Input.Constant.nConst &= ~0x10;
			}
			var.key = var_fbneo_debug_dip_1_6.key;
			if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
			{
				if (strcmp(var.value, "enabled") == 0)
					pgi_debug_dip_1->Input.Constant.nConst |= 0x20;
				else
					pgi_debug_dip_1->Input.Constant.nConst &= ~0x20;
			}
			var.key = var_fbneo_debug_dip_1_7.key;
			if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
			{
				if (strcmp(var.value, "enabled") == 0)
					pgi_debug_dip_1->Input.Constant.nConst |= 0x40;
				else
					pgi_debug_dip_1->Input.Constant.nConst &= ~0x40;
			}
			var.key = var_fbneo_debug_dip_1_8.key;
			if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
			{
				if (strcmp(var.value, "enabled") == 0)
					pgi_debug_dip_1->Input.Constant.nConst |= 0x80;
				else
					pgi_debug_dip_1->Input.Constant.nConst &= ~0x80;
			}
			pgi_debug_dip_1->Input.nVal = pgi_debug_dip_1->Input.Constant.nConst;
			if (pgi_debug_dip_1->Input.pVal)
				*(pgi_debug_dip_1->Input.pVal) = pgi_debug_dip_1->Input.nVal;
		}
		if (pgi_debug_dip_2)
		{
			var.key = var_fbneo_debug_dip_2_1.key;
			if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
			{
				if (strcmp(var.value, "enabled") == 0)
					pgi_debug_dip_2->Input.Constant.nConst |= 0x01;
				else
					pgi_debug_dip_2->Input.Constant.nConst &= ~0x01;
			}
			var.key = var_fbneo_debug_dip_2_2.key;
			if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
			{
				if (strcmp(var.value, "enabled") == 0)
					pgi_debug_dip_2->Input.Constant.nConst |= 0x02;
				else
					pgi_debug_dip_2->Input.Constant.nConst &= ~0x02;
			}
			var.key = var_fbneo_debug_dip_2_3.key;
			if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
			{
				if (strcmp(var.value, "enabled") == 0)
					pgi_debug_dip_2->Input.Constant.nConst |= 0x04;
				else
					pgi_debug_dip_2->Input.Constant.nConst &= ~0x04;
			}
			var.key = var_fbneo_debug_dip_2_4.key;
			if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
			{
				if (strcmp(var.value, "enabled") == 0)
					pgi_debug_dip_2->Input.Constant.nConst |= 0x08;
				else
					pgi_debug_dip_2->Input.Constant.nConst &= ~0x08;
			}
			var.key = var_fbneo_debug_dip_2_5.key;
			if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
			{
				if (strcmp(var.value, "enabled") == 0)
					pgi_debug_dip_2->Input.Constant.nConst |= 0x10;
				else
					pgi_debug_dip_2->Input.Constant.nConst &= ~0x10;
			}
			var.key = var_fbneo_debug_dip_2_6.key;
			if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
			{
				if (strcmp(var.value, "enabled") == 0)
					pgi_debug_dip_2->Input.Constant.nConst |= 0x20;
				else
					pgi_debug_dip_2->Input.Constant.nConst &= ~0x20;
			}
			var.key = var_fbneo_debug_dip_2_7.key;
			if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
			{
				if (strcmp(var.value, "enabled") == 0)
					pgi_debug_dip_2->Input.Constant.nConst |= 0x40;
				else
					pgi_debug_dip_2->Input.Constant.nConst &= ~0x40;
			}
			var.key = var_fbneo_debug_dip_2_8.key;
			if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
			{
				if (strcmp(var.value, "enabled") == 0)
					pgi_debug_dip_2->Input.Constant.nConst |= 0x80;
				else
					pgi_debug_dip_2->Input.Constant.nConst &= ~0x80;
			}
			pgi_debug_dip_2->Input.nVal = pgi_debug_dip_2->Input.Constant.nConst;
			if (pgi_debug_dip_2->Input.pVal)
				*(pgi_debug_dip_2->Input.pVal) = pgi_debug_dip_2->Input.nVal;
		}
	}

	if (BurnDrvGetFlags() & BDF_HISCORE_SUPPORTED)
	{
		var.key = var_fbneo_hiscores.key;
		if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
		{
			if (strcmp(var.value, "enabled") == 0)
				EnableHiscores = true;
			else
				EnableHiscores = false;
		}
	}
	else
	{
		EnableHiscores = false;
	}

	var.key = var_fbneo_allow_patched_romsets.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (strcmp(var.value, "enabled") == 0)
			bPatchedRomsetsEnabled = true;
		else
			bPatchedRomsetsEnabled = false;
	}

	var.key = var_fbneo_allow_ignore_crc.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (strcmp(var.value, "enabled") == 0)
			bAllowIgnoreCrc = true;
		else
			bAllowIgnoreCrc = false;
	}

	if (nGameType != RETRO_GAME_TYPE_NEOCD)
	{
		var.key = var_fbneo_samplerate.key;
		if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
		{
			if (     strcmp(var.value, "48000") == 0)
				g_audio_samplerate = 48000;
			else if (strcmp(var.value, "44100") == 0)
				g_audio_samplerate = 44100;
			else
				g_audio_samplerate = 48000;
		}
	}
	else
	{
		// src/burn/drv/neogeo/neo_run.cpp is mentioning issues with ngcd cdda playback if samplerate isn't 44100
		g_audio_samplerate = 44100;
	}

	var.key = var_fbneo_sample_interpolation.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (     strcmp(var.value, RETRO_SAMPLE_INTERPOLATION_VALUE_2) == 0)
			nInterpolation = 3;
		else if (strcmp(var.value, RETRO_SAMPLE_INTERPOLATION_VALUE_1) == 0)
			nInterpolation = 1;
		else if (strcmp(var.value, "disabled") == 0)
			nInterpolation = 0;
		else
			nInterpolation = 3;
	}

	var.key = var_fbneo_fm_interpolation.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (     strcmp(var.value, RETRO_FM_INTERPOLATION_VALUE_1) == 0)
			nFMInterpolation = 3;
		else if (strcmp(var.value, "disabled") == 0)
			nFMInterpolation = 0;
		else
			nFMInterpolation = 3;
	}

	var.key = var_fbneo_lowpass_filter.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (strcmp(var.value, "enabled") == 0)
			bLowPassFilterEnabled = true;
		else
			bLowPassFilterEnabled = false;
	}

	var.key = var_fbneo_analog_speed.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		nAnalogSpeed = percent_parser(var.value);
	}

	var.key = var_fbneo_socd.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		for (int i = 0; i < 6; i++)
			nSocd[i] = atoi(var.value);
	}

	var.key = var_fbneo_lightgun_crosshair_emulation.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (     strcmp(var.value, RETRO_CROSSHAIR_VALUE_0) == 0)
			nLightgunCrosshairEmulation = 0;
		else if (strcmp(var.value, RETRO_CROSSHAIR_VALUE_1) == 0)
			nLightgunCrosshairEmulation = 1;
		else if (strcmp(var.value, RETRO_CROSSHAIR_VALUE_2) == 0)
			nLightgunCrosshairEmulation = 2;
		RefreshLightgunCrosshair();
	}

#ifdef USE_CYCLONE
	var.key = var_fbneo_cyclone.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (     strcmp(var.value, "enabled" ) == 0)
			bCycloneEnabled = true;
		else if (strcmp(var.value, "disabled") == 0)
			bCycloneEnabled = false;
	}
#endif

#ifdef FBNEO_DEBUG
	var.key = var_fbneo_debug_layer_1.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (     strcmp(var.value, "enabled" ) == 0)
			nBurnLayer |= 1;
		else if (strcmp(var.value, "disabled") == 0)
			nBurnLayer &= ~1;
	}

	var.key = var_fbneo_debug_layer_2.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (     strcmp(var.value, "enabled" ) == 0)
			nBurnLayer |= 2;
		else if (strcmp(var.value, "disabled") == 0)
			nBurnLayer &= ~2;
	}

	var.key = var_fbneo_debug_layer_3.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (     strcmp(var.value, "enabled" ) == 0)
			nBurnLayer |= 4;
		else if (strcmp(var.value, "disabled") == 0)
			nBurnLayer &= ~4;
	}

	var.key = var_fbneo_debug_layer_4.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (     strcmp(var.value, "enabled")  == 0)
			nBurnLayer |= 8;
		else if (strcmp(var.value, "disabled") == 0)
			nBurnLayer &= ~8;
	}

	var.key = var_fbneo_debug_sprite_1.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (     strcmp(var.value, "enabled" ) == 0)
			nSpriteEnable |= 0x01;
		else if (strcmp(var.value, "disabled") == 0)
			nSpriteEnable &= ~0x01;
	}

	var.key = var_fbneo_debug_sprite_2.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (     strcmp(var.value, "enabled" ) == 0)
			nSpriteEnable |= 0x02;
		else if (strcmp(var.value, "disabled") == 0)
			nSpriteEnable &= ~0x02;
	}

	var.key = var_fbneo_debug_sprite_3.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (     strcmp(var.value, "enabled" ) == 0)
			nSpriteEnable |= 0x04;
		else if (strcmp(var.value, "disabled") == 0)
			nSpriteEnable &= ~0x04;
	}

	var.key = var_fbneo_debug_sprite_4.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (     strcmp(var.value, "enabled" ) == 0)
			nSpriteEnable |= 0x08;
		else if (strcmp(var.value, "disabled") == 0)
			nSpriteEnable &= ~0x08;
	}

	var.key = var_fbneo_debug_sprite_5.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (     strcmp(var.value, "enabled" ) == 0)
			nSpriteEnable |= 0x10;
		else if (strcmp(var.value, "disabled") == 0)
			nSpriteEnable &= ~0x10;
	}

	var.key = var_fbneo_debug_sprite_6.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (     strcmp(var.value, "enabled" ) == 0)
			nSpriteEnable |= 0x20;
		else if (strcmp(var.value, "disabled") == 0)
			nSpriteEnable &= ~0x20;
	}

	var.key = var_fbneo_debug_sprite_7.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (     strcmp(var.value, "enabled" ) == 0)
			nSpriteEnable |= 0x40;
		else if (strcmp(var.value, "disabled") == 0)
			nSpriteEnable &= ~0x40;
	}

	var.key = var_fbneo_debug_sprite_8.key;
	if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
	{
		if (     strcmp(var.value, "enabled" ) == 0)
			nSpriteEnable |= 0x80;
		else if (strcmp(var.value, "disabled") == 0)
			nSpriteEnable &= ~0x80;
	}
#endif
}

#ifdef USE_CYCLONE
void SetSekCpuCore()
{
	nSekCpuCore = (bCycloneEnabled ? 0 : 1);
}
#endif


/* load command.dat to core options．[SYSTEM_DIRECTORY]/fbneo/command/command.dat */

// TODO：Mapping tables need to refer to mame source code: https://github.com/mamedev/mame/blob/master/plugins/data/button_char.lua
// command.dat Download: https://www.progettosnaps.net/command
// Try to use unicode to express mame's built-in icons, which cannot be perfectly expressed.
/*👊🦵⚪🛡🐉💥⚡➕➖*/
/*
static SymbolMapping SymbolList[] = {
	{ "_A", "Ⓐ" }, { "_$", "▲" }, { "@L-punch", "[ⓁⓅ]" },
	{ "_B", "Ⓑ" }, { "_#", "▣" }, { "@M-punch", "[ⓂⓅ]" },
	{ "_C", "Ⓒ" }, { "_]", "□" }, { "@S-punch", "[ⓈⓅ]" },
	{ "_D", "Ⓓ" }, { "_[", "■" }, { "@L-kick", "[ⓁⓀ]" },
	{ "_H", "Ⓗ" }, { "_{", "▽" }, { "@M-kick", "[ⓂⓀ]" },
	{ "_Z", "Ⓩ" }, { "_}", "▼" }, { "@S-kick", "[ⓈⓀ]" },
	{ "_a", "①" }, { "_<", "◇" }, { "@3-kick", "[③Ⓚ]" },
	{ "_b", "②" }, { "_>", "◆" }, { "@3-punch", "[③Ⓟ]" },
	{ "_c", "③" }, { "^s", "Ⓢ" }, { "@2-kick", "[②Ⓚ]" },
	{ "_d", "④" }, { "^S", "[Ⓢⓔⓛ]" }, { "@2-punch", "[②Ⓟ]" },
	{ "_e", "⑤" }, { "^E", "[ⓁⓅ]" }, { "@custom1", "①" },
	{ "_f", "⑥" }, { "^F", "[ⓂⓅ]" }, { "@custom2", "②" },
	{ "_g", "⑦" }, { "^G", "[ⓈⓅ]" }, { "@custom3", "③" },
	{ "_h", "⑧" }, { "^H", "[ⓁⓀ]" }, { "@custom4", "④" },
	{ "_i", "⑨" }, { "^I", "[ⓂⓀ]" }, { "@custom5", "⑤" },
	{ "_j", "⑩" }, { "^J", "[ⓈⓀ]" }, { "@custom6", "⑥" },
	{ "_+", "＋" }, { "^T", "[③Ⓚ]" }, { "@custom7", "⑦" },
	{ "_.", "…" }, { "^U", "[③Ⓟ]" }, { "@custom8", "⑧" },
	{ "_1", "↙" }, { "^V", "[②Ⓚ]" }, { "@up", "↑" },
	{ "_2", "↓" }, { "^W", "[②Ⓟ]" }, { "@down", "↓" },
	{ "_3", "↘" }, { "^!", "↳" }, { "@left", "←" },
	{ "_4", "←" }, { "^1", "⇙" }, { "@right", "→" },
	{ "_5", "●" }, { "^2", "⇓" }, { "@lever", "[Ⓟⓝ]" },
	{ "_6", "→" }, { "^3", "⇘" }, { "@nplayer", "[Ⓟⓝ]" },
	{ "_7", "↖" }, { "^4", "⇐" }, { "@1player", "[Ⓟ①]" },
	{ "_8", "↑" }, { "^6", "⇒" }, { "@2player", "[Ⓟ②]" },
	{ "_9", "↗" }, { "^7", "⇖" }, { "@3player", "[Ⓟ③]" },
	{ "_N", "N" }, { "^8", "⇑" }, { "@4player", "[Ⓟ④]" },
	{ "_S", "[Ⓢⓣ]" }, { "^9", "⇗" }, { "@5player", "[Ⓟ⑤]" },
	{ "_P", "Ⓟ" }, { "^M", "[Ⓜⓐⓧ]" }, { "@6player", "[Ⓟ⑥]" },
	{ "_K", "Ⓚ" }, { "^-", "⇥" }, { "@7player", "[Ⓟ⑦]" },
	{ "_G", "Ⓖ" }, { "^=", "⇤" }, { "@8player", "[Ⓟ⑧]" },
	{ "_!", "→" }, { "^*", "[ⓢⓉⓐⓟ]" }, { "@-->","→" },
	{ "_k", "[←◒]" }, { "^?", "[Ⓑⓣⓝ?]" }, { "@==>", "↳" },
	{ "_l", "[→◓]" }, { "@A-button", "Ⓐ" }, { "@hcb", "[←◒]" },
	{ "_m", "[→◒]" }, { "@B-button", "Ⓑ" }, { "@huf", "[→◓]" },
	{ "_n", "[←◓]" }, { "@C-button", "Ⓒ" }, { "@hcf", "[→◒]" },
	{ "_o", "[↓◶]" }, { "@D-button", "Ⓓ" }, { "@hub", "[←◓]" },
	{ "_p", "[←◵]" }, { "@E-button", "Ⓔ" }, { "@qfd", "[↓◶]" },
	{ "_q", "[↑◴]" }, { "@F-button", "Ⓕ" }, { "@qdb", "[←◵]" },
	{ "_r", "[→◷]" }, { "@G-button", "Ⓖ" }, { "@qbu", "[↑◴]" },
	{ "_s", "[↓◵]" }, { "@H-button", "Ⓗ" }, { "@quf", "[→◷]" },
	{ "_t", "[↑◶]" }, { "@I-button", "Ⓘ" }, { "@qbd", "[↓◵]" },
	{ "_u", "[↑◷]" }, { "@J-button", "Ⓙ" }, { "@qdf", "[↑◶]" },
	{ "_v", "[←◴]" }, { "@K-button", "Ⓚ" }, { "@qfu", "[↑◷]" },
	{ "_w", "[↻◯]" }, { "@L-button", "Ⓛ" }, { "@qub", "[←◴]" },
	{ "_x", "[↻◯]" }, { "@M-button", "Ⓜ" }, { "@fdf", "[↻◯]" },
	{ "_y", "[↺◯]" }, { "@N-button", "Ⓝ" }, { "@fub", "[↻◯]" },
	{ "_z", "[↺◯]" }, { "@O-button", "Ⓞ" }, { "@fuf", "[↺◯]" },
	{ "_L", "↠" }, { "@P-button", "Ⓟ" }, { "@fdb", "[↺◯]" },
	{ "_M", "↞" }, { "@Q-button", "Ⓠ" }, { "@xff", "⇥" },
	{ "_Q", "[Ⓓⓡⓐⓖⓞⓝ⇒]" }, { "@R-button", "Ⓡ" }, { "@xbb", "⇤" },
	{ "_R", "[Ⓓⓡⓐⓖⓞⓝ⇐]" }, { "@S-button", "Ⓢ" }, { "@dsf", "[Ⓓⓡⓐⓖⓞⓝ⇒]" },
	{ "_^", "[Ⓐⓘⓡ]" }, { "@T-button", "Ⓣ" }, { "@dsb", "[Ⓓⓡⓐⓖⓞⓝ⇐]" },
	{ "_?", "[Ⓓⓘⓡ]" }, { "@U-button", "Ⓤ" }, { "@AIR", "[Ⓐⓘⓡ]" },
	{ "_X", "[Ⓣⓐⓟ]" }, { "@V-button", "Ⓥ" }, { "@DIR", "[Ⓓⓘⓡ]" },
	{ "_|", "[Ⓙⓤⓜⓟ]" }, { "@W-button", "Ⓦ" }, { "@MAX", "[Ⓜⓐⓧ]" },
	{ "_O", "[Ⓗⓞⓛⓓ]" }, { "@X-button", "Ⓧ" }, { "@TAP", "[Ⓣⓐⓟ]" },
	{ "_-", "[Ⓐⓘⓡ]" }, { "@Y-button", "Ⓨ" }, { "@jump", "[Ⓙⓤⓜⓟ]" },
	{ "_=", "[Ⓢⓠⓤⓐⓣ]" }, { "@Z-button", "Ⓩ" }, { "@hold", "[Ⓗⓞⓛⓓ]" },
	{ "_~", "[Ⓒⓗⓐⓡⓖⓔ]" }, { "@decrease", "⊕" }, { "@air", "[ⓐⓘⓡ]" },
	{ "_`", "•" }, { "@increase", "⊖" }, { "@sit", "[Ⓢⓠⓤⓐⓣ]" },
	{ "_@", "◎" }, { "@BALL", "●" }, { "@close", "⇥" },
	{ "_)", "○" }, { "@start", "[Ⓢⓣ]" }, { "@away", "⇤" },
	{ "_(", "●" }, { "@select", "[Ⓢⓔⓛ]" }, { "@charge", "[Ⓒⓗⓐⓡⓖⓔ]" },
	{ "_*", "☆" }, { "@punch", "Ⓟ" }, { "@tap", "[ⓢⓉⓐⓟ]" },
	{ "_&", "★" }, { "@kick", "Ⓚ" }, { "@button", "[Ⓑⓣⓝ?]" },
	{ "_%", "△" }, { "@guard", "Ⓖ" }
};
*/
// Fail, default Font Unable display all icons. Need Change RetroArch used Font !!!
static SymbolMapping SymbolList[] = {
    { "_A", "\u24B6" }, { "_$", "\u25B2" }, { "@L-punch", "[\u24C1\u24C5]" },
    { "_B", "\u24B7" }, { "_#", "\u25A3" }, { "@M-punch", "[\u24C2\u24C5]" },
    { "_C", "\u24B8" }, { "_]", "\u25A1" }, { "@S-punch", "[\u24C8\u24C5]" },
    { "_D", "\u24B9" }, { "_[", "\u25A0" }, { "@L-kick", "[\u24C1\u24C0]" },
    { "_H", "\u24BD" }, { "_{", "\u25BD" }, { "@M-kick", "[\u24C2\u24C0]" },
    { "_Z", "\u24CF" }, { "_}", "\u25BC" }, { "@S-kick", "[\u24C8\u24C0]" },
    { "_a", "\u2460" }, { "_<", "\u25C7" }, { "@3-kick", "[\u2462\u24C0]" },
    { "_b", "\u2461" }, { "_>", "\u25C6" }, { "@3-punch", "[\u2462\u24C5]" },
    { "_c", "\u2462" }, { "^s", "\u24C8" }, { "@2-kick", "[\u2461\u24C0]" },
    { "_d", "\u2463" }, { "^S", "[\u24C8\u24D4\u24DB]" }, { "@2-punch", "[\u2461\u24C5]" },
    { "_e", "\u2464" }, { "^E", "[\u24C1\u24C5]" }, { "@custom1", "\u2460" },
    { "_f", "\u2465" }, { "^F", "[\u24C2\u24C5]" }, { "@custom2", "\u2461" },
    { "_g", "\u2466" }, { "^G", "[\u24C8\u24C5]" }, { "@custom3", "\u2462" },
    { "_h", "\u2467" }, { "^H", "[\u24C1\u24C0]" }, { "@custom4", "\u2463" },
    { "_i", "\u2468" }, { "^I", "[\u24C2\u24C0]" }, { "@custom5", "\u2464" },
    { "_j", "\u2469" }, { "^J", "[\u24C8\u24C0]" }, { "@custom6", "\u2465" },
    { "_+", "\uFF0B" }, { "^T", "[\u2462\u24C0]" }, { "@custom7", "\u2466" },
    { "_.", "\u2026" }, { "^U", "[\u2462\u24C5]" }, { "@custom8", "\u2467" },
    { "_1", "\u2199" }, { "^V", "[\u2461\u24C0]" }, { "@up", "\u2191" },
    { "_2", "\u2193" }, { "^W", "[\u2461\u24C5]" }, { "@down", "\u2193" },
    { "_3", "\u2198" }, { "^!", "\u21B3" }, { "@left", "\u2190" },
    { "_4", "\u2190" }, { "^1", "\u21D9" }, { "@right", "\u2192" },
    { "_5", "\u25CF" }, { "^2", "\u21D3" }, { "@lever", "[\u24C5\u24DD]" },
    { "_6", "\u2192" }, { "^3", "\u21D8" }, { "@nplayer", "[\u24C5\u24DD]" },
    { "_7", "\u2196" }, { "^4", "\u21D0" }, { "@1player", "[\u24C5\u2460]" },
    { "_8", "\u2191" }, { "^6", "\u21D2" }, { "@2player", "[\u24C5\u2461]" },
    { "_9", "\u2197" }, { "^7", "\u21D6" }, { "@3player", "[\u24C5\u2462]" },
    { "_N", "N" }, { "^8", "\u21D1" }, { "@4player", "[\u24C5\u2463]" },
    { "_S", "[\u24C8\u24E3]" }, { "^9", "\u21D7" }, { "@5player", "[\u24C5\u2464]" },
    { "_P", "\u24C5" }, { "^M", "[\u24C2\u24E0\u24C9]" }, { "@6player", "[\u24C5\u2465]" },
    { "_K", "\u24C0" }, { "^-", "\u21E5" }, { "@7player", "[\u24C5\u2466]" },
    { "_G", "\u24BC" }, { "^=", "\u21E4" }, { "@8player", "[\u24C5\u2467]" },
    { "_!", "\u2192" }, { "^*", "[\u24E3\u24C9\u24D0\u24DF]" }, { "@-->","\u2192" },
    { "_k", "[\u2190\u25D6]" }, { "^?", "[\u24B7\u24E3\u2463]" }, { "@==>", "\u21B3" },
    { "_l", "[\u2192\u25D7]" }, { "@A-button", "\u24B6" }, { "@hcb", "[\u2190\u25D6]" },
    { "_m", "[\u2192\u25D6]" }, { "@B-button", "\u24B7" }, { "@huf", "[\u2192\u25D7]" },
    { "_n", "[\u2190\u25D7]" }, { "@C-button", "\u24B8" }, { "@hcf", "[\u2192\u25D6]" },
    { "_o", "[\u2193\u25D6]" }, { "@D-button", "\u24B9" }, { "@hub", "[\u2190\u25D7]" },
    { "_p", "[\u2190\u25D7]" }, { "@E-button", "\u24BA" }, { "@qfd", "[\u2193\u25D6]" },
    { "_q", "[\u2191\u25D7]" }, { "@F-button", "\u24BB" }, { "@qdb", "[\u2190\u25D7]" },
    { "_r", "[\u2192\u25D7]" }, { "@G-button", "\u24BC" }, { "@qbu", "[\u2191\u25D7]" },
    { "_s", "[\u2193\u25D7]" }, { "@H-button", "\u24BD" }, { "@quf", "[\u2192\u25D7]" },
    { "_t", "[\u2191\u25D6]" }, { "@I-button", "\u24BE" }, { "@qbd", "[\u2193\u25D7]" },
    { "_u", "[\u2191\u25D7]" }, { "@J-button", "\u24BF" }, { "@qdf", "[\u2191\u25D6]" },
    { "_v", "[\u2190\u25D7]" }, { "@K-button", "\u24C0" }, { "@qfu", "[\u2191\u25D7]" },
    { "_w", "[\u21BB\u25CB]" }, { "@L-button", "\u24C1" }, { "@qub", "[\u2190\u25D7]" },
    { "_x", "[\u21BB\u25CB]" }, { "@M-button", "\u24C2" }, { "@fdf", "[\u21BB\u25CB]" },
    { "_y", "[\u21BA\u25CB]" }, { "@N-button", "\u24C3" }, { "@fub", "[\u21BB\u25CB]" },
    { "_z", "[\u21BA\u25CB]" }, { "@O-button", "\u24C4" }, { "@fuf", "[\u21BA\u25CB]" },
    { "_L", "\u21A0" }, { "@P-button", "\u24C5" }, { "@fdb", "[\u21BA\u25CB]" },
    { "_M", "\u219E" }, { "@Q-button", "\u24C6" }, { "@xff", "\u21E5" },
    { "_Q", "[\u24B9\u24E1\u24D0\u24D6\u24DE\u24DD\u21D2]" }, { "@R-button", "\u24C7" }, { "@xbb", "\u21E4" },
    { "_R", "[\u24B9\u24E1\u24D0\u24D6\u24DE\u24DD\u21D0]" }, { "@S-button", "\u24C8" }, { "@dsf", "[\u24B9\u24E1\u24D0\u24D6\u24DE\u24DD\u21D2]" },
    { "_^", "[\u24B6\u24D8\u24E1]" }, { "@T-button", "\u24C9" }, { "@dsb", "[\u24B9\u24E1\u24D0\u24D6\u24DE\u24DD\u21D0]" },
    { "_?", "[\u24B9\u24D8\u24E1]" }, { "@U-button", "\u24CA" }, { "@AIR", "[\u24B6\u24D8\u24E1]" },
    { "_X", "[\u24C9\u24D0\u24DF]" }, { "@V-button", "\u24CB" }, { "@DIR", "[\u24B9\u24D8\u24E1]" },
    { "_|", "[\u24BF\u24E4\u24DC\u24DF]" }, { "@W-button", "\u24CC" }, { "@MAX", "[\u24C2\u24E0\u24C9]" },
    { "_O", "[\u24BD\u24DE\u24DB\u24C3]" }, { "@X-button", "\u24CD" }, { "@TAP", "[\u24C9\u24D0\u24DF]" },
    { "_-", "[\u24B6\u24D8\u24E1]" }, { "@Y-button", "\u24CE" }, { "@jump", "[\u24BF\u24E4\u24DC\u24DF]" },
    { "_=", "[\u24C8\u24E0\u24E4\u24D0\u24C9]" }, { "@Z-button", "\u24CF" }, { "@hold", "[\u24BD\u24DE\u24DB\u24C3]" },
    { "_~", "[\u24B8\u24D7\u24D0\u24E1\u24D6\u24D4]" }, { "@decrease", "\u2295" }, { "@air", "[\u24D0\u24D8\u24E1]" },
    { "_`", "\u2022" }, { "@increase", "\u2296" }, { "@sit", "[\u24C8\u24E0\u24E4\u24D0\u24C9]" },
    { "_@", "\u25CE" }, { "@BALL", "\u25CF" }, { "@close", "\u21E5" },
    { "_)", "\u25CB" }, { "@start", "[\u24C8\u24E3]" }, { "@away", "\u21E4" },
    { "_(", "\u25CF" }, { "@select", "[\u24C8\u24D4\u24DB]" }, { "@charge", "[\u24B8\u24D7\u24D0\u24E1\u24D6\u24D4]" },
    { "_*", "\u2606" }, { "@punch", "\u24C5" }, { "@tap", "[\u24E3\u24C9\u24D0\u24DF]" },
    { "_&", "\u2605" }, { "@kick", "\u24C0" }, { "@button", "[\u24B7\u24E3\u2463]" },
    { "_%", "\u25B3" }, { "@guard", "\u24BC" }
};

static SymbolMapping SymbolList_ChineseAlignment[] = {
	{ "═", "\uFF1D" },  // Full-width equals sign
	{ "│", "\uFF5C" },  // Full-width vertical bar
	{ "  ", "\u3000" }  // Full-width space (double-width space)
	//{ " ", "\u2002" } // Half-width space alternative (non-breaking space)
};

static std::map<std::string, std::string> symbolMap;
static std::map<std::string, std::string> symbolMap_ChineseAlignment;
static void InitializeSymbolMap() {
	int SymbolListSize = sizeof(SymbolList) / sizeof(SymbolList[0]);
	for (int i = 0; i < SymbolListSize; ++i) {
		symbolMap[SymbolList[i].key] = SymbolList[i].value;
	}
}

static void InitializeSymbolMap_ChineseAlignment() {
	int SymbolListSize = sizeof(SymbolList_ChineseAlignment) / sizeof(SymbolList_ChineseAlignment[0]);
	for (int i = 0; i < SymbolListSize; ++i) {
		symbolMap_ChineseAlignment[SymbolList_ChineseAlignment[i].key] = SymbolList_ChineseAlignment[i].value;
	}
}

// Replace custom symbols in MAME move lists based on symbolMap
static std::string ReplaceSymbols(const std::string& input) {
	std::string result = input;
	std::map<std::string, std::string>::iterator it;

	for (it = symbolMap.begin(); it != symbolMap.end(); ++it) {
		size_t pos = 0;
		while ((pos = result.find(it->first, pos)) != std::string::npos) {
			result.replace(pos, it->first.length(), it->second);
			pos += it->second.length();
		}
	}
	return result;
}

//  Replace characters using symbolMap_ChineseAlignment to maintain visual guide alignment
static std::string ReplaceSymbols_ChineseAlignment(const std::string& input) {
	std::string result = input;
	std::map<std::string, std::string>::iterator it;

	for (it = symbolMap_ChineseAlignment.begin(); it != symbolMap_ChineseAlignment.end(); ++it) {
		size_t pos = 0;
		while ((pos = result.find(it->first, pos)) != std::string::npos) {
			result.replace(pos, it->first.length(), it->second);
			pos += it->second.length();
		}
	}
	return result;
}

static std::vector<std::string> CommandDataLine; // Store multiple CommandDataLine entries

static std::string TrimNewLine(char* line) {

	std::string lineStr(line);
	lineStr.erase(std::remove(lineStr.begin(), lineStr.end(), '\r'), lineStr.end()); // delete '\r'
	lineStr.erase(std::remove(lineStr.begin(), lineStr.end(), '\n'), lineStr.end()); // delete '\n'

	return lineStr;
}

static bool ReadCommand_Dat() {
	TCHAR line[4096] = {0};
	TCHAR buffer[256] = {0};
	std::string LineStr;
	std::string LastLineStr;
	std::string token;
	std::string drv_name(BurnDrvGetText(DRV_NAME));
	TCHAR szFilename[MAX_PATH] = _T("");
	bool foundInfo = false; // Flag indicating whether a valid $info line is found (i.e., move list entry for this ROM is detected)
	FILE* cmdFile = NULL;
	bool containsChinese = false;// Check if line contains Chinese characters; replace two spaces with full-width space for alignment in certain visual guides (e.g., kovplus)

	InitializeSymbolMap();

	sprintf(szFilename, "%scommand.dat", szAppCommandPath);
	cmdFile = fopen(szFilename, _T("rt"));
	if (cmdFile == NULL) {
		return false; // open file fail
	}

	while (_fgetts(line, sizeof(line), cmdFile) != NULL) {
		// Skip lines starting with '#'
		if (line[0] == '#') {
			continue;
		}

		// Check if line starts with "$info="
		if (!foundInfo) {
			if (strncmp(line, "$info=", 6) == 0) {
				std::string info_line(line + 6);
				std::istringstream iss(info_line);
				while (std::getline(iss, token, ',')) {
					strncpy(buffer, token.c_str(), sizeof(buffer) - 1);
					buffer[sizeof(buffer) - 1] = '\0';
					token = TrimNewLine(buffer);
					if (token == drv_name) {
						foundInfo = true; // Mark that a valid $info line has been found
						break;
					}
				}
				continue;
			}
		}

		// Continue searching for "$end" and blank lines
		if (foundInfo) {

			// Process the current line
			LineStr = TrimNewLine(line);

			//  Check if current line is "$cmd" and skip it
			if (strncmp(LineStr.c_str(), "$cmd", 4) == 0) {
				// If previous line was "$end", insert empty symbol (equivalent to adding blank line)
				if (strncmp(LastLineStr.c_str(), "$end", 4) == 0) {
					CommandDataLine.push_back("");
				}
				LastLineStr = LineStr;
				continue;
			}
			// Check if current line is "$end" and skip it
			if (strncmp(LineStr.c_str(), "$end", 4) == 0) {
				LastLineStr = LineStr;
				continue;
			}
			// Handle empty lines by adding blank line or breaking extraction as needed
			if (LineStr.empty()) {
				// Prevent consecutive blank lines to save core option space
				if (LastLineStr.empty()) {
					continue;
				}
				CommandDataLine.push_back("");
				LastLineStr = LineStr;
				continue;
			}
			// Terminate immediately upon encountering another "$info=" line
			if (strncmp(LineStr.c_str(), "$info=", 6) == 0) {
				break;
			}
			CommandDataLine.push_back(ReplaceSymbols(LineStr));
			LastLineStr = LineStr;
		}
	}

	while (!CommandDataLine.empty() && CommandDataLine.back().empty()) {
		CommandDataLine.pop_back(); // Remove trailing empty element
	}

	// Check if CommandDataLine contains Chinese characters
	if (!CommandDataLine.empty()) {
		for (int i = 0; i < CommandDataLine.size(); ++i) {
			for (int j = 0; j < CommandDataLine[i].size(); ++j) {
				unsigned char c1 = CommandDataLine[i][j];
				if ((c1 & 0xF0) == 0xE0) { // Verify first byte starts with '1110' (UTF-8 3-byte header)
					if (j + 2 < CommandDataLine[i].size()) {
						unsigned char c2 = CommandDataLine[i][j + 1];
						unsigned char c3 = CommandDataLine[i][j + 2];
						if ((c2 & 0xC0) == 0x80 && (c3 & 0xC0) == 0x80) { // Confirm second/third bytes start with '10' (UTF-8 continuation)
							unsigned int unicode = ((c1 & 0x0F) << 12) | ((c2 & 0x3F) << 6) | (c3 & 0x3F);
							if (unicode >= 0x4E00 && unicode <= 0x9FFF) { // Common Chinese character range
								containsChinese = true;
								break; //Exit detection after finding first Chinese character
							}
						}
					}
				}
			}
		}
	}

	// Perform symbol substitution if Chinese characters exist to maintain visual guide alignment
	if (containsChinese) {
		InitializeSymbolMap_ChineseAlignment();
		for (int i = 0; i < CommandDataLine.size(); ++i) {
			CommandDataLine[i] = ReplaceSymbols_ChineseAlignment(CommandDataLine[i]);
		}
	}

	fclose(cmdFile);
	return foundInfo;
}

int get_command_dat_count() {
	if (CommandDataLine.size() == 0) {
		if (!ReadCommand_Dat()) {
			return 0;
		}
	}
	return CommandDataLine.size();
}

static std::vector<std::string> CommandKeys;
int AddCommandDatOptions(int command_idx_var, retro_core_option_v2_definition *option_defs_us) {
	if (CommandDataLine.size() == 0) {
		if (!ReadCommand_Dat()) {
			return command_idx_var;
		}
	}

	const int maxCommandBlocks = CommandDataLine.size();
	char key[64];

	for (int i = 0; i < maxCommandBlocks; i++) {
		snprintf(key, sizeof(key), "fbneo-commanddat-%d", i);
		CommandKeys.push_back(key);
	}

	for (int j = 0; j < maxCommandBlocks; j++) {
		option_defs_us[command_idx_var].key 				= CommandKeys[j].c_str();
		option_defs_us[command_idx_var].desc 				= " ";
		option_defs_us[command_idx_var].desc_categorized	= CommandDataLine[j].c_str();
		option_defs_us[command_idx_var].info				= NULL;
		option_defs_us[command_idx_var].category_key		= "command_dat";
		option_defs_us[command_idx_var].values[0].value		= " ";
		option_defs_us[command_idx_var].values[1].value		= NULL;
		option_defs_us[command_idx_var].default_value		= " ";
		command_idx_var++;
	}
/*  //Log extracted move list information for debugging purposes
	TCHAR szFilename2[MAX_PATH] = _T("");
	FILE* cmdFile2 = NULL;
	sprintf(szFilename2, "%scommand.log", szAppCommandPath);
	cmdFile2 = fopen(szFilename2, _T("w"));
	if (cmdFile2 != NULL) {
		// Write CommandDataLine contents to a file
		for (int m = 0; m < CommandDataLine.size(); ++m) {
			fwrite(CommandDataLine[m].c_str(), 1, CommandDataLine[m].size(), cmdFile2);
			fwrite("\n", 1, 1, cmdFile2);
		}
		fclose(cmdFile2);
	}
*/
	return command_idx_var;
}
/* load command.dat to core options ended */