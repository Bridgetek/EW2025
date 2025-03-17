/**
 * @file Page_Sticker.c
 * @brief Sticker overlay
 * @author Bridgetek
 * @copyright MIT License (https://opensource.org/licenses/MIT)
 * @date 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "EW2024_Photobooth.h"
#include "EW2024_Photobooth_Utils.h"

#define ST_DIR ASSET_DIR "sticker/"
#define MAX_STICKER_PER_GROUP 42
#define STICKER_GROUP_NUM 7

typedef struct
{
    char name[200];
    int tag;
    uint32_t fmt;
    uint32_t ramg;
    int isLoaded;

    int x, y, w, h;
} sticker_t;

typedef struct
{
    sticker_t *sticker;
    int tag_taskbar;
} taskbar_t;

typedef struct
{
    sticker_t *arr_sticker;
    int size;
    taskbar_t taskbar;
    int start_tag;
} sticker_list_t;

sticker_t animal[] = {
    {.name = ST_DIR "animal/1F996.png"},
    {.name = ST_DIR "animal/1F411.png"},
    {.name = ST_DIR "animal/1F401.png"},
    {.name = ST_DIR "animal/1F402.png"},
    {.name = ST_DIR "animal/1F403.png"},
    {.name = ST_DIR "animal/1F404.png"},
    {.name = ST_DIR "animal/1F405.png"},
    {.name = ST_DIR "animal/1F406.png"},
    {.name = ST_DIR "animal/1F407.png"},
    {.name = ST_DIR "animal/1F408.png"},
    {.name = ST_DIR "animal/1F409.png"},
    {.name = ST_DIR "animal/1F40A.png"},
    {.name = ST_DIR "animal/1F40B.png"},
    {.name = ST_DIR "animal/1F40C.png"},
    {.name = ST_DIR "animal/1F40D.png"},
    {.name = ST_DIR "animal/1F40E.png"},
    {.name = ST_DIR "animal/1F40F.png"},
    {.name = ST_DIR "animal/1F410.png"},
    {.name = ST_DIR "animal/1F412.png"},
    {.name = ST_DIR "animal/1F413.png"},
    {.name = ST_DIR "animal/1F414.png"},
    {.name = ST_DIR "animal/1F415.png"},
    {.name = ST_DIR "animal/1F416.png"},
    {.name = ST_DIR "animal/1F417.png"},
    {.name = ST_DIR "animal/1F418.png"},
    {.name = ST_DIR "animal/1F419.png"},
    {.name = ST_DIR "animal/1F41A.png"},
    {.name = ST_DIR "animal/1F41B.png"},
    {.name = ST_DIR "animal/1F41C.png"},
    {.name = ST_DIR "animal/1F41D.png"},
    {.name = ST_DIR "animal/1F41E.png"},
    {.name = ST_DIR "animal/1F41F.png"},
    {.name = ST_DIR "animal/1F420.png"},
    {.name = ST_DIR "animal/1F421.png"},
    {.name = ST_DIR "animal/1F422.png"},
    {.name = ST_DIR "animal/1F423.png"},
    {.name = ST_DIR "animal/1F424.png"},
    {.name = ST_DIR "animal/1F425.png"},
    {.name = ST_DIR "animal/1F426.png"},
    {.name = ST_DIR "animal/1F427.png"},
    {.name = ST_DIR "animal/1F428.png"},
    {.name = ST_DIR "animal/1F429.png"},
    {.name = ST_DIR "animal/1F42A.png"},
    {.name = ST_DIR "animal/1F42B.png"},
    {.name = ST_DIR "animal/1F42C.png"},
    {.name = ST_DIR "animal/1F42D.png"},
    {.name = ST_DIR "animal/1F42E.png"},
    {.name = ST_DIR "animal/1F42F.png"},
    {.name = ST_DIR "animal/1F430.png"},
    {.name = ST_DIR "animal/1F431.png"},
    {.name = ST_DIR "animal/1F432.png"},
    {.name = ST_DIR "animal/1F433.png"},
    {.name = ST_DIR "animal/1F434.png"},
    {.name = ST_DIR "animal/1F435.png"},
    {.name = ST_DIR "animal/1F436.png"},
    {.name = ST_DIR "animal/1F437.png"},
    {.name = ST_DIR "animal/1F438.png"},
    {.name = ST_DIR "animal/1F439.png"},
    {.name = ST_DIR "animal/1F43A.png"},
    {.name = ST_DIR "animal/1F43B.png"},
    {.name = ST_DIR "animal/1F43C.png"},
    {.name = ST_DIR "animal/1F43D.png"},
    {.name = ST_DIR "animal/1F43E.png"},
    {.name = ST_DIR "animal/1F43F.png"},
    {.name = ST_DIR "animal/1F980.png"},
    {.name = ST_DIR "animal/1F981.png"},
    {.name = ST_DIR "animal/1F982.png"},
    {.name = ST_DIR "animal/1F983.png"},
    {.name = ST_DIR "animal/1F984.png"},
    {.name = ST_DIR "animal/1F985.png"},
    {.name = ST_DIR "animal/1F986.png"},
    {.name = ST_DIR "animal/1F987.png"},
    {.name = ST_DIR "animal/1F988.png"},
    {.name = ST_DIR "animal/1F989.png"},
    {.name = ST_DIR "animal/1F98A.png"},
    {.name = ST_DIR "animal/1F98B.png"},
    {.name = ST_DIR "animal/1F98C.png"},
    {.name = ST_DIR "animal/1F98D.png"},
    {.name = ST_DIR "animal/1F98E.png"},
    {.name = ST_DIR "animal/1F98F.png"},
    {.name = ST_DIR "animal/1F990.png"},
    {.name = ST_DIR "animal/1F991.png"},
    {.name = ST_DIR "animal/1F992.png"},
    {.name = ST_DIR "animal/1F993.png"},
    {.name = ST_DIR "animal/1F994.png"},
    {.name = ST_DIR "animal/1F995.png"},
    {.name = ST_DIR "animal/1F997.png"},
    {.name = ST_DIR "animal/1F998.png"},
    {.name = ST_DIR "animal/1F999.png"},
    {.name = ST_DIR "animal/1F99A.png"},
    {.name = ST_DIR "animal/1F99B.png"},
    {.name = ST_DIR "animal/1F99C.png"},
    {.name = ST_DIR "animal/1F99D.png"},
    {.name = ST_DIR "animal/1F99E.png"},
    {.name = ST_DIR "animal/1F99F.png"},
    {.name = ST_DIR "animal/1F9A1.png"},
    {.name = ST_DIR "animal/1F9A2.png"},
    {.name = ST_DIR "animal/1F9A3.png"},
    {.name = ST_DIR "animal/1F9A4.png"},
    {.name = ST_DIR "animal/1F9A5.png"},
    {.name = ST_DIR "animal/1F9A6.png"},
    {.name = ST_DIR "animal/1F9A7.png"},
    {.name = ST_DIR "animal/1F9A8.png"},
    {.name = ST_DIR "animal/1F9A9.png"},
    {.name = ST_DIR "animal/1F9AA.png"},
    {.name = ST_DIR "animal/1F9AB.png"},
    {.name = ST_DIR "animal/1F9AC.png"},
    {.name = ST_DIR "animal/1F9AD.png"},
    {.name = ST_DIR "animal/1F9AE.png"},
};
sticker_t game[] = {
    {.name = ST_DIR "game/1F3D0.png"},
    {.name = ST_DIR "game/1F5B1.png"},
    {.name = ST_DIR "game/1F3BC.png"},
    {.name = ST_DIR "game/1F3A1.png"},
    {.name = ST_DIR "game/1F3A2.png"},
    {.name = ST_DIR "game/1F3A3.png"},
    {.name = ST_DIR "game/1F3A4.png"},
    {.name = ST_DIR "game/1F3A9.png"},
    {.name = ST_DIR "game/1F3AA.png"},
    {.name = ST_DIR "game/1F3AD.png"},
    {.name = ST_DIR "game/1F3AE.png"},
    {.name = ST_DIR "game/1F3AF.png"},
    {.name = ST_DIR "game/1F3B3.png"},
    {.name = ST_DIR "game/1F3B5.png"},
    {.name = ST_DIR "game/1F3B6.png"},
    {.name = ST_DIR "game/1F3B7.png"},
    {.name = ST_DIR "game/1F3B8.png"},
    {.name = ST_DIR "game/1F3B9.png"},
    {.name = ST_DIR "game/1F3BA.png"},
    {.name = ST_DIR "game/1F3BB.png"},
    {.name = ST_DIR "game/1F3BF.png"},
    {.name = ST_DIR "game/1F3C0.png"},
    {.name = ST_DIR "game/1F3C5.png"},
    {.name = ST_DIR "game/1F3C6.png"},
    {.name = ST_DIR "game/1F3C7.png"},
    {.name = ST_DIR "game/1F3C8.png"},
    {.name = ST_DIR "game/1F3C9.png"},
    {.name = ST_DIR "game/1F3CA.png"},
    {.name = ST_DIR "game/1F3CD.png"},
    {.name = ST_DIR "game/1F3CE.png"},
    {.name = ST_DIR "game/1F3CF.png"},
    {.name = ST_DIR "game/1F3D1.png"},
    {.name = ST_DIR "game/1F3D2.png"},
    {.name = ST_DIR "game/1F3D3.png"},
    {.name = ST_DIR "game/1F3D4.png"},
    {.name = ST_DIR "game/1F3D5.png"},
    {.name = ST_DIR "game/1F3D6.png"},
    {.name = ST_DIR "game/1F4A1.png"},
    {.name = ST_DIR "game/1F4BD.png"},
    {.name = ST_DIR "game/1F4BE.png"},
    {.name = ST_DIR "game/1F4BF.png"},
    {.name = ST_DIR "game/1F4C0.png"},
    {.name = ST_DIR "game/1F4DE.png"},
    {.name = ST_DIR "game/1F4DF.png"},
    {.name = ST_DIR "game/1F4E0.png"},
    {.name = ST_DIR "game/1F4E3.png"},
    {.name = ST_DIR "game/1F4F1.png"},
    {.name = ST_DIR "game/1F4F7.png"},
    {.name = ST_DIR "game/1F4F8.png"},
    {.name = ST_DIR "game/1F4F9.png"},
    {.name = ST_DIR "game/1F4FA.png"},
    {.name = ST_DIR "game/1F4FB.png"},
    {.name = ST_DIR "game/1F4FC.png"},
    {.name = ST_DIR "game/1F4FD.png"},
    {.name = ST_DIR "game/1F5A8.png"},
};
sticker_t face[] = {
    {.name = ST_DIR "face/1F60A.png"},
    {.name = ST_DIR "face/1F611.png"},
    {.name = ST_DIR "face/1F600.png"},
    {.name = ST_DIR "face/1F601.png"},
    {.name = ST_DIR "face/1F602.png"},
    {.name = ST_DIR "face/1F603.png"},
    {.name = ST_DIR "face/1F604.png"},
    {.name = ST_DIR "face/1F605.png"},
    {.name = ST_DIR "face/1F606.png"},
    {.name = ST_DIR "face/1F607.png"},
    {.name = ST_DIR "face/1F608.png"},
    {.name = ST_DIR "face/1F609.png"},
    {.name = ST_DIR "face/1F60B.png"},
    {.name = ST_DIR "face/1F60C.png"},
    {.name = ST_DIR "face/1F60D.png"},
    {.name = ST_DIR "face/1F60E.png"},
    {.name = ST_DIR "face/1F60F.png"},
    {.name = ST_DIR "face/1F610.png"},
    {.name = ST_DIR "face/1F612.png"},
    {.name = ST_DIR "face/1F613.png"},
    {.name = ST_DIR "face/1F614.png"},
    {.name = ST_DIR "face/1F615.png"},
    {.name = ST_DIR "face/1F616.png"},
    {.name = ST_DIR "face/1F617.png"},
    {.name = ST_DIR "face/1F618.png"},
    {.name = ST_DIR "face/1F619.png"},
    {.name = ST_DIR "face/1F61A.png"},
    {.name = ST_DIR "face/1F61B.png"},
    {.name = ST_DIR "face/1F61C.png"},
    {.name = ST_DIR "face/1F61D.png"},
    {.name = ST_DIR "face/1F61E.png"},
    {.name = ST_DIR "face/1F61F.png"},
    {.name = ST_DIR "face/1F620.png"},
    {.name = ST_DIR "face/1F621.png"},
    {.name = ST_DIR "face/1F622.png"},
    {.name = ST_DIR "face/1F623.png"},
    {.name = ST_DIR "face/1F624.png"},
    {.name = ST_DIR "face/1F625.png"},
    {.name = ST_DIR "face/1F626.png"},
    {.name = ST_DIR "face/1F627.png"},
    {.name = ST_DIR "face/1F628.png"},
    {.name = ST_DIR "face/1F629.png"},
    {.name = ST_DIR "face/1F62A.png"},
    {.name = ST_DIR "face/1F62B.png"},
    {.name = ST_DIR "face/1F62C.png"},
    {.name = ST_DIR "face/1F62D.png"},
    {.name = ST_DIR "face/1F62E-200D-1F4A8.png"},
    {.name = ST_DIR "face/1F62E.png"},
    {.name = ST_DIR "face/1F62F.png"},
    {.name = ST_DIR "face/1F630.png"},
    {.name = ST_DIR "face/1F631.png"},
    {.name = ST_DIR "face/1F632.png"},
    {.name = ST_DIR "face/1F633.png"},
    {.name = ST_DIR "face/1F634.png"},
    {.name = ST_DIR "face/1F635-200D-1F4AB.png"},
    {.name = ST_DIR "face/1F635.png"},
    {.name = ST_DIR "face/1F636-200D-1F32B-FE0F.png"},
    {.name = ST_DIR "face/1F636.png"},
    {.name = ST_DIR "face/1F637.png"},
    {.name = ST_DIR "face/1F641.png"},
    {.name = ST_DIR "face/1F642.png"},
    {.name = ST_DIR "face/1F643.png"},
    {.name = ST_DIR "face/1F644.png"},
    {.name = ST_DIR "face/1F910.png"},
    {.name = ST_DIR "face/1F911.png"},
    {.name = ST_DIR "face/1F912.png"},
    {.name = ST_DIR "face/1F913.png"},
    {.name = ST_DIR "face/1F914.png"},
    {.name = ST_DIR "face/1F915.png"},
    {.name = ST_DIR "face/1F917.png"},
    {.name = ST_DIR "face/1F920.png"},
    {.name = ST_DIR "face/1F922.png"},
    {.name = ST_DIR "face/1F923.png"},
    {.name = ST_DIR "face/1F924.png"},
    {.name = ST_DIR "face/1F925.png"},
    {.name = ST_DIR "face/1F92A.png"},
    {.name = ST_DIR "face/1F92B.png"},
    {.name = ST_DIR "face/1F92C.png"},
    {.name = ST_DIR "face/1F92D.png"},
    {.name = ST_DIR "face/1F92E.png"},
    {.name = ST_DIR "face/1F92F.png"},
    {.name = ST_DIR "face/1F970.png"},
    {.name = ST_DIR "face/1F971.png"},
    {.name = ST_DIR "face/1F972.png"},
    {.name = ST_DIR "face/1F973.png"},
    {.name = ST_DIR "face/1F974.png"},
    {.name = ST_DIR "face/1F975.png"},
    {.name = ST_DIR "face/1F976.png"},
    {.name = ST_DIR "face/1F977.png"},
    {.name = ST_DIR "face/1F978.png"},
    {.name = ST_DIR "face/1F979.png"},
    {.name = ST_DIR "face/2639.png"},
    {.name = ST_DIR "face/E280.png"},
    {.name = ST_DIR "face/E281.png"},
    {.name = ST_DIR "face/E282.png"},
    {.name = ST_DIR "face/E283.png"},
};
sticker_t food[] = {
    {.name = ST_DIR "food/1F354.png"},
    {.name = ST_DIR "food/1F371.png"},
    {.name = ST_DIR "food/1F355.png"},
    {.name = ST_DIR "food/1F356.png"},
    {.name = ST_DIR "food/1F357.png"},
    {.name = ST_DIR "food/1F358.png"},
    {.name = ST_DIR "food/1F359.png"},
    {.name = ST_DIR "food/1F360.png"},
    {.name = ST_DIR "food/1F361.png"},
    {.name = ST_DIR "food/1F362.png"},
    {.name = ST_DIR "food/1F363.png"},
    {.name = ST_DIR "food/1F364.png"},
    {.name = ST_DIR "food/1F365.png"},
    {.name = ST_DIR "food/1F366.png"},
    {.name = ST_DIR "food/1F367.png"},
    {.name = ST_DIR "food/1F368.png"},
    {.name = ST_DIR "food/1F369.png"},
    {.name = ST_DIR "food/1F370.png"},
    {.name = ST_DIR "food/1F372.png"},
    {.name = ST_DIR "food/1F373.png"},
    {.name = ST_DIR "food/1F374.png"},
    {.name = ST_DIR "food/1F375.png"},
    {.name = ST_DIR "food/1F376.png"},
    {.name = ST_DIR "food/1F377.png"},
    {.name = ST_DIR "food/1F378.png"},
    {.name = ST_DIR "food/1F379.png"},
    {.name = ST_DIR "food/1F382.png"},
};
sticker_t hand[] = {
    {.name = ST_DIR "hand/1F44B.png"},
    {.name = ST_DIR "hand/1FAF1.png"},
    {.name = ST_DIR "hand/1F446.png"},
    {.name = ST_DIR "hand/1F44A.png"},
    {.name = ST_DIR "hand/1F44C.png"},
    {.name = ST_DIR "hand/1F44D.png"},
    {.name = ST_DIR "hand/1F44E.png"},
    {.name = ST_DIR "hand/1F44F.png"},
    {.name = ST_DIR "hand/1F64C.png"},
    {.name = ST_DIR "hand/1F90F.png"},
    {.name = ST_DIR "hand/1F918.png"},
    {.name = ST_DIR "hand/1F919.png"},
    {.name = ST_DIR "hand/1F91A.png"},
    {.name = ST_DIR "hand/1F91B.png"},
    {.name = ST_DIR "hand/1F91C.png"},
    {.name = ST_DIR "hand/1F91D.png"},
    {.name = ST_DIR "hand/1F91E.png"},
    {.name = ST_DIR "hand/1FAF0.png"},
    {.name = ST_DIR "hand/1FAF2.png"},
    {.name = ST_DIR "hand/1FAF3.png"},
    {.name = ST_DIR "hand/1FAF4.png"},
    {.name = ST_DIR "hand/1FAF5.png"},
    {.name = ST_DIR "hand/1FAF6.png"},
    {.name = ST_DIR "hand/1FAF7.png"},
    {.name = ST_DIR "hand/1FAF8.png"},
    {.name = ST_DIR "hand/261D.png"},
    {.name = ST_DIR "hand/270A.png"},
    {.name = ST_DIR "hand/270B.png"},
    {.name = ST_DIR "hand/270C.png"},
    {.name = ST_DIR "hand/270D.png"},
};
sticker_t vehice[] = {
    {.name = ST_DIR "vehice/1F697.png"},
    {.name = ST_DIR "vehice/1F681.png"},
    {.name = ST_DIR "vehice/1F682.png"},
    {.name = ST_DIR "vehice/1F683.png"},
    {.name = ST_DIR "vehice/1F684.png"},
    {.name = ST_DIR "vehice/1F685.png"},
    {.name = ST_DIR "vehice/1F686.png"},
    {.name = ST_DIR "vehice/1F687.png"},
    {.name = ST_DIR "vehice/1F688.png"},
    {.name = ST_DIR "vehice/1F689.png"},
    {.name = ST_DIR "vehice/1F690.png"},
    {.name = ST_DIR "vehice/1F691.png"},
    {.name = ST_DIR "vehice/1F692.png"},
    {.name = ST_DIR "vehice/1F693.png"},
    {.name = ST_DIR "vehice/1F694.png"},
    {.name = ST_DIR "vehice/1F695.png"},
    {.name = ST_DIR "vehice/1F696.png"},
    {.name = ST_DIR "vehice/1F698.png"},
    {.name = ST_DIR "vehice/1F699.png"},
    {.name = ST_DIR "vehice/1F6E5.png"},
    {.name = ST_DIR "vehice/1F6E9.png"},
    {.name = ST_DIR "vehice/1F6EB.png"},
    {.name = ST_DIR "vehice/1F6EC.png"},
    {.name = ST_DIR "vehice/1F6F0.png"},
    {.name = ST_DIR "vehice/1F6F3.png"},
    {.name = ST_DIR "vehice/1F6F4.png"},
    {.name = ST_DIR "vehice/1F6F5.png"},
    {.name = ST_DIR "vehice/1F6F6.png"},
    {.name = ST_DIR "vehice/1F6F7.png"},
    {.name = ST_DIR "vehice/1F6F8.png"},
    {.name = ST_DIR "vehice/1F6F9.png"},
    {.name = ST_DIR "vehice/1F6FA.png"},
    {.name = ST_DIR "vehice/1F6FB.png"},
    {.name = ST_DIR "vehice/1F6FC.png"},
    {.name = ST_DIR "vehice/1F9BC.png"},
    {.name = ST_DIR "vehice/2708.png"},
    {.name = ST_DIR "vehice/E1C2.png"},
};
sticker_t weather[] = {
    {.name = ST_DIR "weather/26C5.png"},
    {.name = ST_DIR "weather/1F310.png"},
    {.name = ST_DIR "weather/1F319.png"},
    {.name = ST_DIR "weather/1F320.png"},
    {.name = ST_DIR "weather/1F321.png"},
    {.name = ST_DIR "weather/1F324.png"},
    {.name = ST_DIR "weather/1F325.png"},
    {.name = ST_DIR "weather/1F326.png"},
    {.name = ST_DIR "weather/1F327.png"},
    {.name = ST_DIR "weather/1F328.png"},
    {.name = ST_DIR "weather/1F329.png"},
    {.name = ST_DIR "weather/1F331.png"},
    {.name = ST_DIR "weather/1F332.png"},
    {.name = ST_DIR "weather/1F333.png"},
    {.name = ST_DIR "weather/1F334.png"},
    {.name = ST_DIR "weather/1F335.png"},
    {.name = ST_DIR "weather/1F336.png"},
    {.name = ST_DIR "weather/1F337.png"},
    {.name = ST_DIR "weather/1F338.png"},
    {.name = ST_DIR "weather/1F339.png"},
    {.name = ST_DIR "weather/1F340.png"},
    {.name = ST_DIR "weather/1F341.png"},
    {.name = ST_DIR "weather/1F342.png"},
    {.name = ST_DIR "weather/1F343.png"},
    {.name = ST_DIR "weather/1F344.png"},
    {.name = ST_DIR "weather/1F345.png"},
    {.name = ST_DIR "weather/1F346.png"},
    {.name = ST_DIR "weather/1F347.png"},
    {.name = ST_DIR "weather/1F348.png"},
    {.name = ST_DIR "weather/1F349.png"},
    {.name = ST_DIR "weather/1F350.png"},
    {.name = ST_DIR "weather/1F351.png"},
    {.name = ST_DIR "weather/1F352.png"},
    {.name = ST_DIR "weather/1F353.png"},
    {.name = ST_DIR "weather/2600.png"},
    {.name = ST_DIR "weather/2601.png"},
    {.name = ST_DIR "weather/26C8.png"},
};

#define sticker_num_animal (min(MAX_STICKER_PER_GROUP, (sizeof(animal) / sizeof(animal[0]))))
#define sticker_num_game (min(MAX_STICKER_PER_GROUP, (sizeof(game) / sizeof(game[0]))))
#define sticker_num_face (min(MAX_STICKER_PER_GROUP, (sizeof(face) / sizeof(face[0]))))
#define sticker_num_food (min(MAX_STICKER_PER_GROUP, (sizeof(food) / sizeof(food[0]))))
#define sticker_num_hand (min(MAX_STICKER_PER_GROUP, (sizeof(hand) / sizeof(hand[0]))))
#define sticker_num_vehice (min(MAX_STICKER_PER_GROUP, (sizeof(vehice) / sizeof(vehice[0]))))
#define sticker_num_weather (min(MAX_STICKER_PER_GROUP, (sizeof(weather) / sizeof(weather[0]))))

#define sticker_start_animal 0
#define sticker_start_game sticker_start_animal + sticker_num_animal
#define sticker_start_face sticker_start_game + sticker_num_game
#define sticker_start_food sticker_start_face + sticker_num_face
#define sticker_start_hand sticker_start_food + sticker_num_food
#define sticker_start_vehice sticker_start_hand + sticker_num_hand
#define sticker_start_weather sticker_start_vehice + sticker_num_vehice

#define sticker_w 72
#define sticker_format ARGB4
#define sticker_load_format IMG_FMT_PNG

sticker_list_t sticker_list[] = {
    {face, sticker_num_face, &face[0]},
    {hand, sticker_num_hand, &hand[0]},
    {animal, sticker_num_animal, &animal[0]},
    {game, sticker_num_game, &game[0]},
    {food, sticker_num_food, &food[0]},
    {vehice, sticker_num_vehice, &vehice[0]},
    {weather, sticker_num_weather, &weather[0]},
};

#define sticker_list_num (sizeof(sticker_list) / sizeof(sticker_list[0]))
#define sticker_num (STICKER_GROUP_NUM * MAX_STICKER_PER_GROUP)

const int scale_ratio = 300;
const int scale_adjust_xy = -scale_ratio * sticker_w / 100 / 2;

typedef struct
{
    sticker_t *icon;
    uint32_t tag;
    int x, y;
    int dragx, dragy;
} sticker_onscreen_t;

sticker_onscreen_t sticker_onscreen[STICKER_GROUP_NUM * MAX_STICKER_PER_GROUP] = {{0}};
static int onscreen_count = 0, taskbar_nth_selected = 0;
static int sticker_dragging = -1;

int tag_sticker_start = 0, tag_sticker_end = 0;
int tag_add_sticker = 0;

extern uint32_t taskbar_icon_x;
extern uint32_t taskbar_icon_y;
extern int32_t lvdsrx_w;
extern int32_t lvdsrx_h;

static int insert_sticker_onscreen(sticker_t *icon)
{
    Gesture_Touch_t *ges = utils_gestureGet();
    if (onscreen_count < (sticker_num - 1))
    {
        sticker_onscreen[onscreen_count].icon = icon;
        sticker_onscreen[onscreen_count].x = icon->x;
        sticker_onscreen[onscreen_count].y = icon->y;

        sticker_dragging = onscreen_count;

        onscreen_count++;
        return 0;
    }
    return -1;
}

static int remove_sticker_onscreen(uint32_t tagval)
{
    int i;
    for (i = 0; i < onscreen_count && i < sticker_num; i++)
    {
        if (sticker_onscreen[i].tag == tagval)
        {
            // shift left
            for (int j = i; j < onscreen_count - 1 && j < sticker_num - 1; j++)
            {
                sticker_onscreen[j].icon = sticker_onscreen[j + 1].icon;
                sticker_onscreen[j].x = sticker_onscreen[j + 1].x;
                sticker_onscreen[j].y = sticker_onscreen[j + 1].y;
                sticker_onscreen[j].dragx = sticker_onscreen[j + 1].dragx;
                sticker_onscreen[j].dragy = sticker_onscreen[j + 1].dragy;
            }
            sticker_onscreen[onscreen_count - 1].icon = 0;
            onscreen_count--;
            return 0;
        }
    }
    return -1;
}

static void event_on_icon_deselected()
{
    Gesture_Touch_t *ges = utils_gestureGet();

    // double tap on the icon to remove it
    if (!ges->isDoubleTapTag)
        return;

    if (ges->tagPressed <= 0)
        return;

    if (ges->tagPressed < sticker_onscreen[0].tag || ges->tagPressed > sticker_onscreen[sticker_num - 1].tag)
    {
        return;
    }

    remove_sticker_onscreen(ges->tagPressed);
}

static void event_on_icon_selected()
{
    // select and dragging
    Gesture_Touch_t *ges = utils_gestureGet();
    if (ges->tagPressed < tag_sticker_start || ges->tagPressed > tag_sticker_end)
    {
        return;
    }

    if (taskbar_nth_selected < 0)
    {
        return;
    }

    if (0 <= sticker_dragging && sticker_dragging <= onscreen_count)
    {
        return;
    }
    // select new icon
    int sticker_nth = ges->tagPressed - sticker_list[taskbar_nth_selected].start_tag;

    sticker_t *icon = &sticker_list[taskbar_nth_selected].arr_sticker[sticker_nth];
    icon->x = ges->touchX + scale_adjust_xy;
    icon->y = ges->touchY + scale_adjust_xy;

    insert_sticker_onscreen(icon);
}
static void event_on_icon_draging()
{
    Gesture_Touch_t *ges = utils_gestureGet();

    // end the dragging
    if (!ges->isTouch && 0 <= sticker_dragging && sticker_dragging <= onscreen_count)
    {
        sticker_onscreen_t *icon = &sticker_onscreen[sticker_dragging];
        icon->x += icon->dragx;
        icon->y += icon->dragy;
        icon->dragx = 0;
        icon->dragy = 0;
        sticker_dragging = -1;
        return;
    }

    // dragging
    if (0 <= sticker_dragging && sticker_dragging <= onscreen_count)
    {
        sticker_onscreen_t *icon = &sticker_onscreen[sticker_dragging];
        icon->dragx = ges->distanceX;
        icon->dragy = ges->distanceY;
        return;
    }

    // start a new dragging
    if (sticker_onscreen[0].tag <= ges->tagPressed && ges->tagPressed <= sticker_onscreen[sticker_num - 1].tag)
    {
        sticker_dragging = max(0, min(sticker_num - 1, ges->tagPressed - sticker_onscreen[0].tag));
        sticker_onscreen_t *icon = &sticker_onscreen[sticker_dragging];
        icon->dragx = 0;
        icon->dragy = 0;
        return;
    }
}

static void event_on_taskbar_taskbar_selected()
{
    Gesture_Touch_t *ges = utils_gestureGet();
    for (int i = 0; i < sticker_list_num; i++)
    {
        sticker_list_t *arr = &sticker_list[i];
        if (ges->tagPressed1 == arr->taskbar.tag_taskbar)
        {
            if (taskbar_nth_selected == i)
            {
                return;
            }

            taskbar_nth_selected = i;
            return;
        }
    }
}

static int event_on_add_icon()
{
    Gesture_Touch_t *ges = utils_gestureGet();
    if (ges->tagPressed1 == tag_add_sticker)
    {
        if (taskbar_nth_selected >= 0)
        {
            taskbar_nth_selected = -1;
        }
        else
        {
            taskbar_nth_selected = 0;
        }
    }
    return PAGE_CONTINUE;
}
static int event()
{
    event_on_taskbar_taskbar_selected();
    event_on_icon_selected();
    event_on_icon_deselected();
    event_on_icon_draging();
    event_on_add_icon();

    return PAGE_CONTINUE;
}

static void draw_sticker_xy(sticker_t *st, int x, int y, int tag, int scale)
{
    Image im;
    im.w = sticker_w;
    im.h = sticker_w;
    im.bitmap_format = st->fmt;
    im.tagval = st->tag;
    im.is_loaded = st->isLoaded;
    im.ptr = st->ramg;
    im.scale = scale;
    im.bitmap_handler = BMHL_NONE;
    im.sd_path = st->name;
    utils_drawImageXyTag(PHOST, &im, x, y, tag);
}

static int drawing_icon_more(int nth)
{
#define GAP 10
    sticker_list_t *arr = &sticker_list[nth];
    sticker_t *st_arr = arr->arr_sticker;

    box sticker_box = {.w = lvdsrx_w / 3, .h = lvdsrx_h / 2, .x = 0, .y = 0};
    sticker_box.x = lvdsrx_w / 2 - sticker_box.w / 2;
    sticker_box.y = lvdsrx_h / 2 - sticker_box.h / 2;

    int x = sticker_box.x;
    int y = sticker_box.y;
    int w = sticker_box.w;
    int h = sticker_box.h;
    int col_num = max(1, w / (GAP + sticker_w));

    int panel_padding = 10;

    tag(PHOST, 0);
    color_rgb(PHOST, 100, 100, 100);
    color_a(PHOST, 100);
    utils_drawBox(&sticker_box);
    color_rgb(PHOST, 255, 255, 255);
    color_a(PHOST, 255);

    for (int j = 0; j < arr->size; j++)
    {
        if (j > MAX_STICKER_PER_GROUP)
            continue;
        sticker_t *st = &st_arr[j];
        x = sticker_box.x + panel_padding + j % col_num * (sticker_w + GAP);
        y = sticker_box.y + panel_padding + j / col_num * (GAP + sticker_w);
        draw_sticker_xy(st, x, y, st->tag, 100);
    }
}
static int drawing_icon_on_taskbar()
{
    box sticker_box = {.w = lvdsrx_w / 3, .h = lvdsrx_h / 2, .x = 0, .y = 0};
    sticker_box.x = lvdsrx_w / 2 - sticker_box.w / 2;
    sticker_box.y = lvdsrx_h / 2 - sticker_box.h / 2;

    int panel_padding = 10;
    int x = sticker_box.x;
    int y = sticker_box.y - sticker_w;
    int col_num = sticker_list_num;
    int gapx = (sticker_box.w - panel_padding * 2) / max(1, sticker_list_num);

    tag(PHOST, 0);
    color_rgb(PHOST, 0xff, 0xe5, 0xcc);
    color_a(PHOST, 255);
    utils_drawRect(x, y, sticker_box.w, sticker_w);
    color_rgb(PHOST, 255, 255, 255);

    for (int i = 0; i < sticker_list_num; i++)
    {
        if (i == taskbar_nth_selected)
        {
            tag(PHOST, sticker_list[i].taskbar.tag_taskbar);
            color_rgb(PHOST, 0xff, 0xbf, 0x80);
            utils_drawBoxText_4(PHOST, x, y, 72, 72);
            color_rgb(PHOST, 255, 255, 255);
        }
        draw_sticker_xy(sticker_list[i].taskbar.sticker, x + panel_padding, y, sticker_list[i].taskbar.tag_taskbar, 100);
        x += gapx;
    }
}
static void drawing_icon_selected()
{
    Gesture_Touch_t *ges = utils_gestureGet();

    for (int i = 0; i < onscreen_count && i < STICKER_GROUP_NUM * MAX_STICKER_PER_GROUP; i++)
    {
        if (sticker_onscreen[i].icon != 0)
        {
            sticker_onscreen_t *onscreen = &sticker_onscreen[i];
            utils_scale(scale_ratio);

            int x = max(min(1920, onscreen->x + onscreen->dragx), 0);
            int y = max(min(1080, onscreen->y + onscreen->dragy), 0);
            draw_sticker_xy(onscreen->icon, x, y, onscreen->tag, scale_ratio);
            utils_scale(100);
        }
    }
}
static int drawing()
{
    drawing_icon_selected();
    if (taskbar_nth_selected != -1 && sticker_dragging == -1)
    {
        drawing_icon_more(taskbar_nth_selected);
        drawing_icon_on_taskbar();
    }

    sticker_t *add_icon = &hand[0];
    draw_sticker_xy(add_icon, taskbar_icon_x, taskbar_icon_y, tag_add_sticker, 100);
    return 0;
}

static void page_sticker_init_one()
{
    static bool is_page_sticker_init = 0;
    if (is_page_sticker_init == 1)
        return;
    is_page_sticker_init = 1;
#define ENABLE_BG 1

    tag_sticker_start = tag_counter - 1;
    // Load all image into ramg
    int loaded_num = 0;
    for (int i = 0; i < sticker_list_num; i++)
    {
        sticker_list_t *arr = &sticker_list[i];
        sticker_t *st_arr = arr->arr_sticker;
        for (int j = 0; j < arr->size; j++)
        {
            sticker_t *st = &st_arr[j];
            Image im;

            st->ramg = 0;
            st->tag = 0;
            st->isLoaded = 0;
            if (j > MAX_STICKER_PER_GROUP)
                continue;

            im.sd_path = st->name;
            im.w = sticker_w;
            im.h = sticker_w;
            im.fmt = sticker_load_format;
            im.bitmap_format = sticker_format;
            im.bitmap_handler = BMHL_NONE;
            im.tagval = TAG_INC;
            st->isLoaded = 0;
            im.opt = 0;
            uint32_t image_load_status = 0;
#if ENABLE_BG
            {
                char msg[200];
                utils_displayStartColor(PHOST, (uint8_t[]){255, 255, 255}, (uint8_t[]){0, 0, 255});
                utils_drawImageCenter(PHOST, IMG_LOGO_1920, 0, 0, PHOST->Width, PHOST->Height);

                int percentage = 100 * loaded_num / sticker_num;
                snprintf(msg, 200, "Loading %d sticker (%d%%)", loaded_num + 1, percentage);
                EVE_CoCmd_text(PHOST, PHOST->Width / 2, PHOST->Height / 2 + 250, 30, OPT_CENTER, msg);

                // find image name
                int strpos = 0;
                int slen = min(99, strlen(im.sd_path));
                for (int j = slen - 1; j >= 0; j--)
                {
                    if (im.sd_path[j] == '/' || im.sd_path[j] == '\\')
                    {
                        strpos = slen - j;
                        break;
                    }
                }
                snprintf(msg, 200, "Loading sticker %s", &im.sd_path[strpos]);
                EVE_CoCmd_text(PHOST, 50, PHOST->Height - 50, 30, 0, msg);
            }
#endif
            image_load_status = utils_loadImageFromSdCard(PHOST, &im);
            st->ramg = im.ptr;
            st->tag = im.tagval;
            st->fmt = im.bitmap_format;
            st->isLoaded = 1;
            st->w = im.w;
            st->h = im.h;

            if (image_load_status || !im.is_loaded) // error with the SDcard reading
            {
                printf("ERROR loading image!\r\n");
                //  utils_drawBox
                EVE_CoCmd_text(PHOST, PHOST->Width / 2 - 300, PHOST->Height - 500, 32, 0, "ERROR loading image!");

                utils_displayEnd(PHOST);
                while (1)
                {
                    EVE_sleep(1000);
                }
            }

#if ENABLE_BG
            utils_displayEnd(PHOST);
            loaded_num++;
#endif
        }
        arr->start_tag = st_arr[0].tag;
    }
    tag_sticker_end = tag_counter;
    tag_add_sticker = utils_tagAlloc(TAG_INC);
    ;

    // taskbar icon has a different tag
    for (int i = 0; i < sticker_list_num; i++)
    {
        sticker_list_t *arr = &sticker_list[i];
        arr->taskbar.tag_taskbar = utils_tagAlloc(TAG_INC);
    }

    // onscreen has a different tag
    for (int i = 0; i < sticker_num; i++)
    {
        sticker_onscreen[i].icon = 0;
        sticker_onscreen[i].x = 0;
        sticker_onscreen[i].y = 0;
        sticker_onscreen[i].tag = utils_tagAlloc(TAG_INC);
    }
}

static void page_sticker_deinit_one()
{
    static bool is_page_sticker_deinit = 0;
    if (is_page_sticker_deinit == 1)
        return;
    is_page_sticker_deinit = 1;
}

void page_sticker_load()
{
}

int page_sticker_draw()
{
    int ret = event();
    drawing();
    return ret;
}

void page_sticker_init()
{
    page_sticker_init_one();
}

void page_sticker_deinit()
{
    page_sticker_deinit_one();
}
