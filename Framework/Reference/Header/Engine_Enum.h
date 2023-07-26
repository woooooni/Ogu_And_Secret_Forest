#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum WINMODE { MODE_FULL, MODE_WIN };
	// Dynamic 컴포넌트 경우 매 프레임마다 갱신해야하는 컴포넌트 집단
	enum COMPONENTID { ID_DYNAMIC, ID_STATIC, ID_END };

	enum MATRIX_INFO {	INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };
	enum ROTATION { ROT_X, ROT_Y, ROT_Z, ROT_END };

	enum TEXTUREID { TEX_NORMAL, TEX_CUBE, TEX_END };

	enum RENDERID { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_EFFECT, RENDER_UI, RENDER_END };

	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };
	enum MOUSEMOVESTATE {	DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

	enum class COMPONENT_TYPE	
	{ 
		COM_BUFFER,
		COM_TRANSFORM, 
		COM_CIRCLE_COLLIDER,
		COM_BOX_COLLIDER,
		COM_TEXTURE,
		COM_ANIMATOR,
		COM_RIGIDBODY,
		COM_END
	};

	
	enum class LAYER_TYPE { CAMERA, PLAYER, TERRAIN, ENVIRONMENT, MONSTER, INTERACTION_OBJ, EFFECT, UI, LAYER_END };
	enum class SCENE_TYPE { LOADING, TEST, LOGO, TUTORIAL_VILLAGE, MONKEY_FOREST1, MONKEY_FOREST2, MONKEY_VILLAGE, MONKEY_FOREST3 ,SUNGOLEM_CAVE1, MOON_FOREST1, SILK_WORM, TOOL, SCENE_END };

	//Object
	enum class OBJ_TYPE { OBJ_SKYBOX, OBJ_CAMERA, OBJ_ENVIRONMENT, OBJ_PORTAL, OBJ_TILE, OBJ_PLAYER, OBJ_SPAWNER, OBJ_MONSTER, OBJ_EFFECT, OBJ_INTERACTION, OBJ_BULLET, OBJ_ITEM, OBJ_UI,OBJ_NEXUS, OBJ_END };
	enum class OBJ_DIR { DIR_U, DIR_D, DIR_L, DIR_R, DIR_LU, DIR_RU, DIR_LD, DIR_RD, DIR_END };
	enum class OBJ_HITTYPE { HIT_ONCE, HIT_REPEAT, HIT_BREAK, HIT_END };
	enum class JELLY_COLOR { CYAN, MAGENTA, YELLOW, BLUE, GREEN, RED, JELLY_END };
	enum class JELLY_COLLOR_NORMAL { CYAN, MAGENTA, YELLOW, JELLY_NORMALEND };
	enum class JELLY_COLLOR_COMBINE{ BLUE = 1, GREEN, RED, JELLY_COMBINEEND};
	enum class LIGHTPUZZLE_TYPE{};
	enum class GRASS_TYPE{ JUNGLEBUSH_BRIGHT, JUNGLEBUSH_DARK, ROUNDBUSH, SHARP, HEALTHBUSH, SPEEDBUSH, LONGGRASS, GLOWING_REED, GLOWING_REED_RED, GRASS_END   };
	enum class HITOBJ_TYPE { BUG, MONKEY, MOUSE, HIT_END };

	// Collision
	enum class COLLIDER_TYPE { COLLIDER_BOX, COLLIDER_CIRCLE, COLLIDER_END };
	enum class COLLISION_DIR{ NOT_COLLIDE, DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT, DIR_FRONT, DIR_BACK, DIR_END };
	
	// Transform
	enum class DIR_TRANSFORM { DIR_X, DIR_Y, DIR_Z, DIR_END };

	// Monster & Boss
	enum class MONSTER_STATE { IDLE, MOVE, REGEN, ATTACK, DIE, STUN ,DEFFENCEMODE ,END };
	enum class SUNGOLEM_STATE { REGEN, IDLE, DIRTY, MOVE, ATTACK, DIE, END };

	enum class SILKWORM_STATE { REGEN, IDLE, READY, DOWN, ATTACK, DIE, END };

	// Player
	enum class PLAYER_STATE { IDLE, MOVE, RUN, ROLL, JUMP, SWING, HIT ,LIFT, DOWN, GAMEOVER, PUSH, GETTIEM, BALLOONFLY, DRAWING , SKILL, DANCE ,END };
	enum class PLAYER_SKILL { NONE, GOLEMFIST, TURTLE, DRILL, LIGHTNING ,MISSILE, SKILLEND};
	enum class DRILL_STATE {START, INGROUND, OUTGROUND, SKILLEND };
	enum class BALLOONFLY_STATE { FLYREADY, FLYING, FLYINGEND, END };
	enum class LIFT_STATE {LIFTREADY, LIFTUP, LIFTDOWN, LIFTIDLE, LIFTWALK, LIFTEND};
	enum class PLAYER_HAT {NONE, LIGHT, GOLEMFIST, LIGHTNING, TURTLE, DRILL, MISSILE, HATEND};

	// Camera
	enum class CAMERA_STATE { GAME, TOOL, CUT_SCENE, END };
	enum class SKYBOX_TYPE {  SKY_END };


	// Quest
	enum class QUEST_PROGRESS { BEFORE, CONTINUE, COMPLETE, PROGRESS_END };
	enum class QUEST_TYPE { CONVERSATION, HUNTING, END };

	// NPC
	enum class NPC_CODE { NPC_SHEEP, NPC_COW, NPC_ARTIST, NPC_MOM, NPC_CHIEF, NPC_ELITE, NPC_DANCETEACHER, NPC_DANCER, NPC_VILLAGER_MONKEY, NPC_ORANGI, NPC_WHOLESALER, NPC_FISHING, NPC_NUE_HERO, NPC_MOON_VILLAGER, CODE_END };

	enum COLOR_STATE{
		COLOR_RED=0,
		COLOR_GREEN,
		COLOR_BLUE,
		COLOR_END
	};

	typedef enum class COLLIDE_STATE
	{
		COLLIDE_WALL,
		COLLIDE_PLAYER,
		COLLIDE_MONSTER,
		COLLIDE_GRAB,
		COLLIDE_PUSH,
		COLLIDE_BREAK,
		COLLIDE_BULLET,
		COLLIDE_BOMB,
		COLLIDE_BALPAN,
		COLLIDE_SWING,
		COLLIDE_TRIGGER,
		COLLIDE_BOSS,
		COLLIDE_EFFECT,
		COLLIDE_ITEM,
		COLLIDE_PORTAL,
		COLLIDE_NEXUS,
		COLLIDE_END
	} COLLISION_GROUP;

	enum class COLLIDER_PLAYER
	{
		COLLIDER_ATTACK,
		COLLIDER_GRAB,
		COLLIDER_END
	};

	enum class OBJ_ID 
	{
		//PLAYER
		PLAYER,

		// CAMERA
		CAMERA,

		// TERRAIN
		TERRAIN,

		// SKYBOX
		SKYBOX,

		// ENVIRONMENT
		GRASS, HOUSE, TILE, TREE, PROP, PORTAL,

		// ITEM
		ITEM, COIN_ITEM, PLAYER_HAT, PLAYER_HAT_MONKEY,

		// SKILL
		MONSTER_SKILL, PLAYER_SKILL, BUG_BALL,

		// NPC
		NPC,

		//MONSTER & BOSS
		BLUE_BEATLE, RED_BEATLE, GREEN_BEATLE, DESERT_RHINO, TRASH_BIG, TRASH_SLIME, TRASH_BUMMER, TRASH_FAST, SPIT_CACTUS, MORTH_MAGE, ROLLING_BUG, CUPA, SUN_GOLLEM, SILK_WORM,
		
		// INTERACTION OBJ
		NEAR_REACT, NEAR_REACT_MOON, NEAR_REACT_MUSHROOM, PUSH_STONE, BALPAN_OBJ, BLOCK_OBJ, HIT_OBJ, BUG_STATUE, BUTTERFLY, CATAPULT, 
		JELLY_BOMB, JELLY_BOMB_CREATOR, JELLY_STONE, JELLY_COMBINED, LIGHT_FLOWER, LIGHT_PUZZLE, LIGHT_PUZZLEBASE, LIGHT_PUZZLE_PIECE, LIGHT_PUZZLE_TERRAIN,
		TURRET, BENCH, CLEAR_BOMB, CLEAR_FIELD, NEXUS,
		
		// UI
		UI,
		//EFFECT
		EFFECT,
		ID_END
	};

	enum class FONT_TYPE
	{
		CAFE24_SURROUND_AIR,
		CAFE24_SURROUND_BOLD,
		TYPE_END
	};

	enum class EVENT_TYPE 
	{
		SCENE_CHANGE,
		ADD_OBJECT,
		DELETE_OBJECT,
		EVENT_END
	};

	enum class ITEM_TYPE
	{
		EQUIPMENT,
		CONSUMPTION,
		ETC,
		IMPORTANT,
		COIN,
		TYPE_END
	};

	enum class EFFECT_TYPE
	{
		HEAL,
		POWER,
		SPEED,
		POISON,
		EFFECT_END
	};

	enum class ITEM_CODE
	{
		//CONSUME
		HP_SMALL,
		HP_MIDDLE,
		HP_BIG,
		SPEED_SMALL,
		SPEED_MIDDLE,
		SPEED_BIG,


		//ETC
		LEAF,
		TWIG,

		//ETC_SPECIAL
		BUTTERFLY,
		SASUM,

		//EQUIP
		HAT_DRILL,
		HAT_LIGHT,
		HAT_MASK,
		HAT_MISSLE,
		HAT_MONKEY,
		HAT_TURTLE,

		//IMPORTANT
		DRAWING_CROSSROAD,
		DRAWING_MONKEYSTATUE,
		DRAWING_MONKEYTOWN,
		DRAWING_COLORS,
		ITEM_END,

		CONSUME_END = LEAF,
		EQUIP_END = DRAWING_CROSSROAD,
		ETC_END = HAT_DRILL,
		IMPORTANT_END = ITEM_END


	};
	
	// KeyBoard
	enum class KEY_STATE
	{
		NONE,
		TAP,
		HOLD,
		AWAY
	};

	enum class KEY
	{
		LEFT_ARROW,
		RIGHT_ARROW,
		UP_ARROW,
		DOWN_ARROW,
		TAB,
		OPEN_SQUARE_BRACKET,
		CLOSE_SQUARE_BRACKET,
		F1, F2, F3, F4, F5, F6, F7, F8, F9,
		INSERT, DEL, HOME, END, PAGE_UP, PAGE_DOWN,
		NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9,
		Q, W, E, R, T, Y, U, I, O, P,
		A, S, D, F, G, H, J, K, L,
		Z, X, C, V, B, N, M,
		ALT, CTRL, SHIFT,
		SPACE, ENTER, ESC,
		LBTN, RBTN,
		TYPE_END
	};

	enum CHANNELID
	{
		SOUND_UI,
		SOUND_VOICE,
		SOUND_SKILL,
		SOUND_EFFECT_PLAYER,
		SOUND_EFFECT_MONSTER,
		SOUND_EFFECT_MONSTER2,
		SOUND_EFFECT_MONSTER3,
		SOUND_EFFECT_MONSTER4,
		SOUND_EFFECT_MONSTER5,
		SOUND_EFFECT_ENVIRONMENT,
		SOUND_EFFECT_INTERACTION,
		SOUND_EFFECT_SYSTEM,
		SOUND_HIT,
		SOUND_MONSTER_HIT,
		SOUND_BGM,
		SOUND_ITEM,
		SOUND_JUMP,
		SOUND_BOSS,
		MAXCHANNEL
	};
}
#endif // Engine_Enum_h__
