// This is an automatically generated file.
// Do not edit directly.
//
#include "SMB.hpp"

void SMBEngine::code(int mode)
{
    switch (mode)
    {
    case 0:
        loadConstantData();
        goto Start;
    case 1:
        goto NonMaskableInterrupt;
    }

    // ;SMBDIS.ASM - A COMPREHENSIVE SUPER MARIO BROS. DISASSEMBLY
    // ;by doppelganger (doppelheathen@gmail.com)
    // ;This file is provided for your own use as-is.  It will require the character rom data
    // ;and an iNES file header to get it to work.
    // ;There are so many people I have to thank for this, that taking all the credit for
    // ;myself would be an unforgivable act of arrogance. Without their help this would
    // ;probably not be possible.  So I thank all the peeps in the nesdev scene whose insight into
    // ;the 6502 and the NES helped me learn how it works (you guys know who you are, there's no
    // ;way I could have done this without your help), as well as the authors of x816 and SMB
    // ;Utility, and the reverse-engineers who did the original Super Mario Bros. Hacking Project,
    // ;which I compared notes with but did not copy from.  Last but certainly not least, I thank
    // ;Nintendo for creating this game and the NES, without which this disassembly would
    // ;only be theory.
    // ;Assembles with x816.
    // ;-------------------------------------------------------------------------------------
    // ;DEFINES
    // ;NES specific hardware defines
    // ; GAME SPECIFIC DEFINES
    // ;sound related defines
    // ;-------------------------------------------------------------------------------------
    // ;CONSTANTS
    // ;sound effects constants
    // Sfx_SmallJump         = %10000000
    // Sfx_Flagpole          = %01000000
    // Sfx_Fireball          = %00100000
    // Sfx_PipeDown_Injury   = %00010000
    // Sfx_EnemySmack        = %00001000
    // Sfx_EnemyStomp        = %00000100
    // Sfx_Bump              = %00000010
    // Sfx_BigJump           = %00000001
    // Sfx_BowserFall        = %10000000
    // Sfx_ExtraLife         = %01000000
    // Sfx_PowerUpGrab       = %00100000
    // Sfx_TimerTick         = %00010000
    // Sfx_Blast             = %00001000
    // Sfx_GrowVine          = %00000100
    // Sfx_GrowPowerUp       = %00000010
    // Sfx_CoinGrab          = %00000001
    // Sfx_BowserFlame       = %00000010
    // Sfx_BrickShatter      = %00000001
    // ;music constants
    // Silence               = %10000000
    // StarPowerMusic        = %01000000
    // PipeIntroMusic        = %00100000
    // CloudMusic            = %00010000
    // CastleMusic           = %00001000
    // UndergroundMusic      = %00000100
    // WaterMusic            = %00000010
    // GroundMusic           = %00000001
    // TimeRunningOutMusic   = %01000000
    // EndOfLevelMusic       = %00100000
    // AltGameOverMusic      = %00010000
    // EndOfCastleMusic      = %00001000
    // VictoryMusic          = %00000100
    // GameOverMusic         = %00000010
    // DeathMusic            = %00000001
    // ;enemy object constants
    // ;other constants
    // WarmBootOffset        = <$07d6
    // ColdBootOffset        = <$07fe
    // SwimTileRepOffset     = PlayerGraphicsTable + $9e
    // MusicHeaderOffsetData = MusicHeaderData - 1
    // MHD                   = MusicHeaderData
    // A_Button              = %10000000
    // B_Button              = %01000000
    // Select_Button         = %00100000
    // Start_Button          = %00010000
    // Up_Dir                = %00001000
    // Down_Dir              = %00000100
    // Left_Dir              = %00000010
    // Right_Dir             = %00000001
    // ;-------------------------------------------------------------------------------------
    // ;DIRECTIVES
    // .p02
    // .org $8000
    // ;-------------------------------------------------------------------------------------

Start:
    /* sei */
    /* cld */
    a = 0x10;
    writeData(PPU_CTRL_REG1, a);
    x = 0xff;
    s = x;

VBlank1:
    a = M(PPU_STATUS);
    if (!n)
        goto VBlank1;

VBlank2:
    a = M(PPU_STATUS);
    if (!n)
        goto VBlank2;
    y = 0xfe;
    x = 0x05;

WBootCheck:
    a = M(TopScoreDisplay + x);
    compare(a, 0x0a);
    if (c)
        goto ColdBoot;
    --x;
    if (!n)
        goto WBootCheck;
    a = M(WarmBootValidation);
    compare(a, 0xa5);
    if (!z)
        goto ColdBoot;
    y = 0xd6;

ColdBoot:
    JSR(InitializeMemory, 0);
    writeData(0x4011, a);
    writeData(OperMode, a);
    a = 0xa5;
    writeData(WarmBootValidation, a);
    writeData(PseudoRandomBitReg, a);
    a = 0x0f;
    writeData(SND_MASTERCTRL_REG, a);
    a = 0x06;
    writeData(PPU_CTRL_REG2, a);
    JSR(MoveAllSpritesOffscreen, 1);
    JSR(InitializeNameTables, 2);
    ++M(DisableScreenFlag);
    a = M(Mirror_PPU_CTRL_REG1);
    a |= 0x80;
    JSR(WritePPUReg1, 3);

EndlessLoop:
    return;
    // ;-------------------------------------------------------------------------------------
    // ;$00 - vram buffer address table low, also used for pseudorandom bit
    // ;$01 - vram buffer address table high

NonMaskableInterrupt:
    a = M(Mirror_PPU_CTRL_REG1);
    a &= 0x7f;
    writeData(Mirror_PPU_CTRL_REG1, a);
    a &= 0x7e;
    writeData(PPU_CTRL_REG1, a);
    a = M(Mirror_PPU_CTRL_REG2);
    a &= 0xe6;
    y = M(DisableScreenFlag);
    if (!z)
        goto ScreenOff;
    a = M(Mirror_PPU_CTRL_REG2);
    a |= 0x1e;

ScreenOff:
    writeData(Mirror_PPU_CTRL_REG2, a);
    a &= 0xe7;
    writeData(PPU_CTRL_REG2, a);
    x = M(PPU_STATUS);
    a = 0x00;
    JSR(InitScroll, 4);
    writeData(PPU_SPR_ADDR, a);
    a = 0x02;
    writeData(SPR_DMA, a);
    x = M(VRAM_Buffer_AddrCtrl);
    a = M(VRAM_AddrTable_Low + x);
    writeData(TitleScreenModeValue, a);
    a = M(VRAM_AddrTable_High + x);
    writeData(GameModeValue, a);
    JSR(UpdateScreen, 5);
    y = 0x00;
    x = M(VRAM_Buffer_AddrCtrl);
    compare(x, 0x06);
    if (!z)
        goto InitBuffer;
    ++y;

InitBuffer:
    x = M(VRAM_Buffer_Offset + y);
    a = 0x00;
    writeData(VRAM_Buffer1_Offset + x, a);
    writeData(VRAM_Buffer1 + x, a);
    writeData(VRAM_Buffer_AddrCtrl, a);
    a = M(Mirror_PPU_CTRL_REG2);
    writeData(PPU_CTRL_REG2, a);
    JSR(SoundEngine, 6);
    JSR(ReadJoypads, 7);
    JSR(PauseRoutine, 8);
    JSR(UpdateTopScore, 9);
    a = M(GamePauseStatus);
    a >>= 1;
    if (c)
        goto PauseSkip;
    a = M(TimerControl);
    if (z)
        goto DecTimers;
    --M(TimerControl);
    if (!z)
        goto NoDecTimers;

DecTimers:
    x = 0x14;
    --M(IntervalTimerControl);
    if (!n)
        goto DecTimersLoop;
    a = 0x14;
    writeData(IntervalTimerControl, a);
    x = 0x23;

DecTimersLoop:
    a = M(SelectTimer + x);
    if (z)
        goto SkipExpTimer;
    --M(SelectTimer + x);

SkipExpTimer:
    --x;
    if (!n)
        goto DecTimersLoop;

NoDecTimers:
    ++M(TallEnemy);

PauseSkip:
    x = 0x00;
    y = 0x07;
    a = M(PseudoRandomBitReg);
    a &= 0x02;
    writeData(TitleScreenModeValue, a);
    a = M(0x07a8);
    a &= 0x02;
    a ^= M(TitleScreenModeValue);
    c = 0;
    if (z)
        goto RotPRandomBit;
    c = 1;

RotPRandomBit:
    M(PseudoRandomBitReg + x).ror();
    ++x;
    --y;
    if (!z)
        goto RotPRandomBit;
    a = M(Sprite0HitDetectFlag);
    if (z)
        goto SkipSprite0;

Sprite0Clr:
    a = M(PPU_STATUS);
    a &= 0x40;
    if (!z)
        goto Sprite0Clr;
    a = M(GamePauseStatus);
    a >>= 1;
    if (c)
        goto Sprite0Hit;
    JSR(MoveSpritesOffscreen, 10);
    JSR(SpriteShuffler, 11);

Sprite0Hit:
    a = M(PPU_STATUS);
    a &= 0x40;
    if (z)
        goto Sprite0Hit;
    y = 0x14;

HBlankDelay:
    --y;
    if (!z)
        goto HBlankDelay;

SkipSprite0:
    a = M(HorizontalScroll);
    writeData(PPU_SCROLL_REG, a);
    a = M(VerticalScroll);
    writeData(PPU_SCROLL_REG, a);
    a = M(Mirror_PPU_CTRL_REG1);
    pha();
    writeData(PPU_CTRL_REG1, a);
    a = M(GamePauseStatus);
    a >>= 1;
    if (c)
        goto SkipMainOper;
    JSR(OperModeExecutionTree, 12);

SkipMainOper:
    a = M(PPU_STATUS);
    pla();
    a |= 0x80;
    writeData(PPU_CTRL_REG1, a);
    return;
    // ;-------------------------------------------------------------------------------------

PauseRoutine:
    a = M(OperMode);
    compare(a, 0x02);
    if (z)
        goto ChkPauseTimer;
    compare(a, 0x01);
    if (!z)
        goto ExitPause;
    a = M(OperMode_Task);
    compare(a, 0x03);
    if (!z)
        goto ExitPause;

ChkPauseTimer:
    a = M(GamePauseTimer);
    if (z)
        goto ChkStart;
    --M(GamePauseTimer);
    goto Return;

ChkStart:
    a = M(SavedJoypad1Bits);
    a &= 0x10;
    if (z)
        goto ClrPauseTimer;
    a = M(GamePauseStatus);
    a &= 0x80;
    if (!z)
        goto ExitPause;
    a = 0x2b;
    writeData(GamePauseTimer, a);
    a = M(GamePauseStatus);
    y = a;
    ++y;
    writeData(PauseSoundQueue, y);
    a ^= 0x01;
    a |= 0x80;
    if (!z)
        goto SetPause;

ClrPauseTimer:
    a = M(GamePauseStatus);
    a &= 0x7f;

SetPause:
    writeData(GamePauseStatus, a);

ExitPause:
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$00 - used for preset value

SpriteShuffler:
    y = M(AreaType);
    a = 0x28;
    writeData(TitleScreenModeValue, a);
    x = 0x0e;

ShuffleLoop:
    a = M(Player_SprDataOffset + x);
    compare(a, M(TitleScreenModeValue));
    if (!c)
        goto NextSprOffset;
    y = M(SprShuffleAmtOffset);
    c = 0;
    a += M(SprShuffleAmt + y);
    if (!c)
        goto StrSprOffset;
    c = 0;
    a += M(TitleScreenModeValue);

StrSprOffset:
    writeData(Player_SprDataOffset + x, a);

NextSprOffset:
    --x;
    if (!n)
        goto ShuffleLoop;
    x = M(SprShuffleAmtOffset);
    ++x;
    compare(x, 0x03);
    if (!z)
        goto SetAmtOffset;
    x = 0x00;

SetAmtOffset:
    writeData(SprShuffleAmtOffset, x);
    x = 0x08;
    y = 0x02;

SetMiscOffset:
    a = M(0x06e9 + y);
    writeData(FBall_SprDataOffset + x, a);
    c = 0;
    a += 0x08;
    writeData(0x06f2 + x, a);
    c = 0;
    a += 0x08;
    writeData(Misc_SprDataOffset + x, a);
    --x;
    --x;
    --x;
    --y;
    if (!n)
        goto SetMiscOffset;
    goto Return;
    // ;-------------------------------------------------------------------------------------

OperModeExecutionTree:
    a = M(OperMode);
    switch (a) {
        case 0: goto TitleScreenMode;
        case 1: goto GameMode;
        case 2: goto VictoryMode;
        case 3: goto GameOverMode;
    }

MoveAllSpritesOffscreen:
    y = 0x00;
    bit(M(Fireball_BoundBoxCtrl));
    goto loc_8225;

MoveSpritesOffscreen:
    y = 0x04;
loc_8225:
    a = 0xf8;

SprInitLoop:
    writeData(Sprite_Y_Position + y, a);
    ++y;
    ++y;
    ++y;
    ++y;
    if (!z)
        goto SprInitLoop;
    goto Return;
    // ;-------------------------------------------------------------------------------------

TitleScreenMode:
    a = M(OperMode_Task);
    switch (a) {
        case 0: goto InitializeGame;
        case 1: goto ScreenRoutines;
        case 2: goto PrimaryGameSetup;
        case 3: goto GameMenuRoutine;
    }

GameMenuRoutine:
    y = 0x00;
    a = M(SavedJoypad1Bits);
    a |= M(SavedJoypad2Bits);
    compare(a, 0x10);
    if (z)
        goto StartGame;
    compare(a, 0x90);
    if (!z)
        goto ChkSelect;

StartGame:
    goto ChkContinue;

ChkSelect:
    compare(a, 0x20);
    if (z)
        goto SelectBLogic;
    x = M(DemoTimer);
    if (!z)
        goto ChkWorldSel;
    writeData(SelectTimer, a);
    JSR(DemoEngine, 13);
    if (c)
        goto ResetTitle;
    goto RunDemo;

ChkWorldSel:
    x = M(WorldSelectEnableFlag);
    if (z)
        goto NullJoypad;
    compare(a, 0x40);
    if (!z)
        goto NullJoypad;
    ++y;

SelectBLogic:
    a = M(DemoTimer);
    if (z)
        goto ResetTitle;
    a = 0x18;
    writeData(DemoTimer, a);
    a = M(SelectTimer);
    if (!z)
        goto NullJoypad;
    a = 0x10;
    writeData(SelectTimer, a);
    compare(y, 0x01);
    if (z)
        goto IncWorldSel;
    a = M(NumberOfPlayers);
    a ^= 0x01;
    writeData(NumberOfPlayers, a);
    JSR(DrawMushroomIcon, 14);
    goto NullJoypad;

IncWorldSel:
    x = M(WorldSelectNumber);
    ++x;
    a = x;
    a &= 0x07;
    writeData(WorldSelectNumber, a);
    JSR(GoContinue, 15);

UpdateShroom:
    a = M(WSelectBufferTemplate + x);
    writeData(VRAM_Buffer1_Offset + x, a);
    ++x;
    compare(x, 0x06);
    if (n)
        goto UpdateShroom;
    y = M(WorldNumber);
    ++y;
    writeData(0x0304, y);

NullJoypad:
    a = 0x00;
    writeData(SavedJoypad1Bits, a);

RunDemo:
    JSR(GameCoreRoutine, 16);
    a = M(GreenParatroopaJump);
    compare(a, 0x06);
    if (!z)
        goto ExitMenu;

ResetTitle:
    a = 0x00;
    writeData(OperMode, a);
    writeData(OperMode_Task, a);
    writeData(Sprite0HitDetectFlag, a);
    ++M(DisableScreenFlag);
    goto Return;

ChkContinue:
    y = M(DemoTimer);
    if (z)
        goto ResetTitle;
    a <<= 1;
    if (!c)
        goto StartWorld1;
    a = M(ContinueWorld);
    JSR(GoContinue, 17);

StartWorld1:
    JSR(LoadAreaPointer, 18);
    ++M(Hidden1UpFlag);
    ++M(OffScr_Hidden1UpFlag);
    ++M(FetchNewGameTimerFlag);
    ++M(OperMode);
    a = M(WorldSelectEnableFlag);
    writeData(PrimaryHardMode, a);
    a = 0x00;
    writeData(OperMode_Task, a);
    writeData(DemoTimer, a);
    x = 0x17;
    a = 0x00;

InitScores:
    writeData(PlayerScoreDisplay + x, a);
    --x;
    if (!n)
        goto InitScores;

ExitMenu:
    goto Return;

GoContinue:
    writeData(WorldNumber, a);
    writeData(OffScr_WorldNumber, a);
    x = 0x00;
    writeData(AreaNumber, x);
    writeData(OffScr_AreaNumber, x);
    goto Return;
    // ;-------------------------------------------------------------------------------------

DrawMushroomIcon:
    y = 0x07;

IconDataRead:
    a = M(MushroomIconData + y);
    writeData(VRAM_Buffer1_Offset + y, a);
    --y;
    if (!n)
        goto IconDataRead;
    a = M(NumberOfPlayers);
    if (z)
        goto ExitIcon;
    a = 0x24;
    writeData(0x0304, a);
    a = 0xce;
    writeData(0x0306, a);

ExitIcon:
    goto Return;
    // ;-------------------------------------------------------------------------------------

DemoEngine:
    x = M(DemoAction);
    a = M(DemoActionTimer);
    if (!z)
        goto DoAction;
    ++x;
    ++M(DemoAction);
    c = 1;
    a = M(0x8354 + x);
    writeData(DemoActionTimer, a);
    if (z)
        goto DemoOver;

DoAction:
    a = M(0x833f + x);
    writeData(SavedJoypad1Bits, a);
    --M(DemoActionTimer);
    c = 0;

DemoOver:
    goto Return;
    // ;-------------------------------------------------------------------------------------

VictoryMode:
    JSR(VictoryModeSubroutines, 19);
    a = M(OperMode_Task);
    if (z)
        goto AutoPlayer;
    x = 0x00;
    writeData(BulletBill_FrenzyVar, x);
    JSR(EnemiesAndLoopsCore, 20);

AutoPlayer:
    JSR(RelativePlayerPosition, 21);
    goto PlayerGfxHandler;

VictoryModeSubroutines:
    a = M(OperMode_Task);
    switch (a) {
        case 0: goto BridgeCollapse;
        case 1: goto SetupVictoryMode;
        case 2: goto PlayerVictoryWalk;
        case 3: goto PrintVictoryMessages;
        case 4: goto PlayerEndWorld;
    }

SetupVictoryMode:
    x = M(ScreenRight_PageLoc);
    ++x;
    writeData(FirebarSpinDirection, x);
    a = 0x08;
    writeData(EventMusicQueue, a);
    goto IncModeTask_B;
    // ;-------------------------------------------------------------------------------------

PlayerVictoryWalk:
    y = 0x00;
    writeData(RetainerObject, y);
    a = M(Player_PageLoc);
    compare(a, M(FirebarSpinDirection));
    if (!z)
        goto PerformWalk;
    a = M(Player_X_Position);
    compare(a, 0x60);
    if (c)
        goto DontWalk;

PerformWalk:
    ++M(RetainerObject);
    ++y;

DontWalk:
    a = y;
    JSR(AutoControlPlayer, 22);
    a = M(ScreenLeft_PageLoc);
    compare(a, M(FirebarSpinDirection));
    if (z)
        goto ExitVWalk;
    a = M(ScrollFractional);
    c = 0;
    a += 0x80;
    writeData(ScrollFractional, a);
    a = 0x01;
    a += 0x00;
    y = a;
    JSR(ScrollScreen, 23);
    JSR(UpdScrollVar, 24);
    ++M(RetainerObject);

ExitVWalk:
    a = M(RetainerObject);
    if (z)
        goto IncModeTask_A;
    goto Return;
    // ;-------------------------------------------------------------------------------------

PrintVictoryMessages:
    a = M(SecondaryMsgCounter);
    if (!z)
        goto IncMsgCounter;
    a = M(PrimaryMsgCounter);
    if (z)
        goto ThankPlayer;
    compare(a, 0x09);
    if (c)
        goto IncMsgCounter;
    y = M(WorldNumber);
    compare(y, 0x07);
    if (!z)
        goto MRetainerMsg;
    compare(a, 0x03);
    if (!c)
        goto IncMsgCounter;
    a -= 0x01;
    goto ThankPlayer;

MRetainerMsg:
    compare(a, 0x02);
    if (!c)
        goto IncMsgCounter;

ThankPlayer:
    y = a;
    if (!z)
        goto SecondPartMsg;
    a = M(CurrentPlayer);
    if (z)
        goto EvalForMusic;
    ++y;
    if (!z)
        goto EvalForMusic;

SecondPartMsg:
    ++y;
    a = M(WorldNumber);
    compare(a, 0x07);
    if (z)
        goto EvalForMusic;
    --y;
    compare(y, 0x04);
    if (c)
        goto SetEndTimer;
    compare(y, 0x03);
    if (c)
        goto IncMsgCounter;

EvalForMusic:
    compare(y, 0x03);
    if (!z)
        goto PrintMsg;
    a = 0x04;
    writeData(EventMusicQueue, a);

PrintMsg:
    a = y;
    c = 0;
    a += 0x0c;
    writeData(VRAM_Buffer_AddrCtrl, a);

IncMsgCounter:
    a = M(SecondaryMsgCounter);
    c = 0;
    a += 0x04;
    writeData(SecondaryMsgCounter, a);
    a = M(PrimaryMsgCounter);
    a += 0x00;
    writeData(PrimaryMsgCounter, a);
    compare(a, 0x07);

SetEndTimer:
    if (!c)
        goto ExitMsgs;
    a = 0x06;
    writeData(WorldEndTimer, a);

IncModeTask_A:
    ++M(OperMode_Task);

ExitMsgs:
    goto Return;
    // ;-------------------------------------------------------------------------------------

PlayerEndWorld:
    a = M(WorldEndTimer);
    if (!z)
        goto EndExitOne;
    y = M(WorldNumber);
    compare(y, 0x07);
    if (c)
        goto EndChkBButton;
    a = 0x00;
    writeData(AreaNumber, a);
    writeData(LevelNumber, a);
    writeData(OperMode_Task, a);
    ++M(WorldNumber);
    JSR(LoadAreaPointer, 25);
    ++M(FetchNewGameTimerFlag);
    a = 0x01;
    writeData(OperMode, a);

EndExitOne:
    goto Return;

EndChkBButton:
    a = M(SavedJoypad1Bits);
    a |= M(SavedJoypad2Bits);
    a &= 0x40;
    if (z)
        goto EndExitTwo;
    a = 0x01;
    writeData(WorldSelectEnableFlag, a);
    a = 0xff;
    writeData(NumberofLives, a);
    JSR(TerminateGame, 26);

EndExitTwo:
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;data is used as tiles for numbers
    // ;that appear when you defeat enemies
    // ;high nybble is digit number, low nybble is number to
    // ;add to the digit of the player's score

FloateyNumbersRoutine:
    a = M(FloateyNum_Control + x);
    if (z)
        goto EndExitOne;
    compare(a, 0x0b);
    if (!c)
        goto ChkNumTimer;
    a = 0x0b;
    writeData(FloateyNum_Control + x, a);

ChkNumTimer:
    y = a;
    a = M(FloateyNum_Timer + x);
    if (!z)
        goto DecNumTimer;
    writeData(FloateyNum_Control + x, a);
    goto Return;

DecNumTimer:
    --M(FloateyNum_Timer + x);
    compare(a, 0x2b);
    if (!z)
        goto ChkTallEnemy;
    compare(y, 0x0b);
    if (!z)
        goto LoadNumTiles;
    ++M(NumberofLives);
    a = 0x40;
    writeData(Square2SoundQueue, a);

LoadNumTiles:
    a = M(ScoreUpdateData + y);
    a >>= 1;
    a >>= 1;
    a >>= 1;
    a >>= 1;
    x = a;
    a = M(ScoreUpdateData + y);
    a &= 0x0f;
    writeData(DigitModifier + x, a);
    JSR(AddToScore, 27);

ChkTallEnemy:
    y = M(Enemy_SprDataOffset + x);
    a = M(Fireworks + x);
    compare(a, 0x12);
    if (z)
        goto FloateyPart;
    compare(a, 0x0d);
    if (z)
        goto FloateyPart;
    compare(a, 0x05);
    if (z)
        goto GetAltOffset;
    compare(a, 0x0a);
    if (z)
        goto FloateyPart;
    compare(a, 0x0b);
    if (z)
        goto FloateyPart;
    compare(a, 0x09);
    if (c)
        goto GetAltOffset;
    a = M(Enemy_State + x);
    compare(a, 0x02);
    if (c)
        goto FloateyPart;

GetAltOffset:
    x = M(SprDataOffset_Ctrl);
    y = M(Alt_SprDataOffset + x);
    x = M(BulletBill_FrenzyVar);

FloateyPart:
    a = M(FloateyNum_Y_Pos + x);
    compare(a, 0x18);
    if (!c)
        goto SetupNumSpr;
    a -= 0x01;
    writeData(FloateyNum_Y_Pos + x, a);

SetupNumSpr:
    a = M(FloateyNum_Y_Pos + x);
    a -= 0x08;
    JSR(DumpTwoSpr, 28);
    a = M(FloateyNum_X_Pos + x);
    writeData(Sprite_X_Position + y, a);
    c = 0;
    a += 0x08;
    writeData(0x0207 + y, a);
    a = 0x02;
    writeData(Sprite_Attributes + y, a);
    writeData(0x0206 + y, a);
    a = M(FloateyNum_Control + x);
    a <<= 1;
    x = a;
    a = M(FloateyNumTileData + x);
    writeData(Sprite_Tilenumber + y, a);
    a = M(0x84a0 + x);
    writeData(0x0205 + y, a);
    x = M(BulletBill_FrenzyVar);
    goto Return;
    // ;-------------------------------------------------------------------------------------

ScreenRoutines:
    a = M(ScreenRoutineTask);
    switch (a) {
        case 0: goto InitScreen;
        case 1: goto SetupIntermediate;
        case 2: goto WriteTopStatusLine;
        case 3: goto WriteBottomStatusLine;
        case 4: goto DisplayTimeUp;
        case 5: goto ResetSpritesAndScreenTimer;
        case 6: goto DisplayIntermediate;
        case 7: goto ResetSpritesAndScreenTimer;
        case 8: goto AreaParserTaskControl;
        case 9: goto GetAreaPalette;
        case 10: goto GetBackgroundColor;
        case 11: goto GetAlternatePalette1;
        case 12: goto DrawTitleScreen;
        case 13: goto ClearBuffersDrawIcon;
        case 14: goto WriteTopScore;
    }

InitScreen:
    JSR(MoveAllSpritesOffscreen, 29);
    JSR(InitializeNameTables, 30);
    a = M(OperMode);
    if (z)
        goto NextSubtask;
    x = 0x03;
    goto SetVRAMAddr_A;
    // ;-------------------------------------------------------------------------------------

SetupIntermediate:
    a = M(BackgroundColorCtrl);
    pha();
    a = M(PlayerStatus);
    pha();
    a = 0x00;
    writeData(PlayerStatus, a);
    a = 0x02;
    writeData(BackgroundColorCtrl, a);
    JSR(GetPlayerColors, 31);
    pla();
    writeData(PlayerStatus, a);
    pla();
    writeData(BackgroundColorCtrl, a);
    goto IncSubtask;
    // ;-------------------------------------------------------------------------------------

GetAreaPalette:
    y = M(AreaType);
    x = M(AreaPalette + y);

SetVRAMAddr_A:
    writeData(VRAM_Buffer_AddrCtrl, x);

NextSubtask:
    goto IncSubtask;
    // ;-------------------------------------------------------------------------------------
    // ;$00 - used as temp counter in GetPlayerColors

GetBackgroundColor:
    y = M(BackgroundColorCtrl);
    if (z)
        goto NoBGColor;
    a = M(0x85c7 + y);
    writeData(VRAM_Buffer_AddrCtrl, a);

NoBGColor:
    ++M(ScreenRoutineTask);

GetPlayerColors:
    x = M(VRAM_Buffer1_Offset);
    y = 0x00;
    a = M(CurrentPlayer);
    if (z)
        goto ChkFiery;
    y = 0x04;

ChkFiery:
    a = M(PlayerStatus);
    compare(a, 0x02);
    if (!z)
        goto StartClrGet;
    y = 0x08;

StartClrGet:
    a = 0x03;
    writeData(TitleScreenModeValue, a);

ClrGetLoop:
    a = M(PlayerColors + y);
    writeData(0x0304 + x, a);
    ++y;
    ++x;
    --M(TitleScreenModeValue);
    if (!n)
        goto ClrGetLoop;
    x = M(VRAM_Buffer1_Offset);
    y = M(BackgroundColorCtrl);
    if (!z)
        goto SetBGColor;
    y = M(AreaType);

SetBGColor:
    a = M(BackgroundColors + y);
    writeData(0x0304 + x, a);
    a = 0x3f;
    writeData(VRAM_Buffer1 + x, a);
    a = 0x10;
    writeData(0x0302 + x, a);
    a = 0x04;
    writeData(0x0303 + x, a);
    a = 0x00;
    writeData(0x0308 + x, a);
    a = x;
    c = 0;
    a += 0x07;

SetVRAMOffset:
    writeData(VRAM_Buffer1_Offset, a);
    goto Return;
    // ;-------------------------------------------------------------------------------------

GetAlternatePalette1:
    a = M(AreaStyle);
    compare(a, 0x01);
    if (!z)
        goto NoAltPal;
    a = 0x0b;

SetVRAMAddr_B:
    writeData(VRAM_Buffer_AddrCtrl, a);

NoAltPal:
    goto IncSubtask;
    // ;-------------------------------------------------------------------------------------

WriteTopStatusLine:
    a = 0x00;
    JSR(WriteGameText, 32);
    goto IncSubtask;
    // ;-------------------------------------------------------------------------------------

WriteBottomStatusLine:
    JSR(GetSBNybbles, 33);
    x = M(VRAM_Buffer1_Offset);
    a = 0x20;
    writeData(VRAM_Buffer1 + x, a);
    a = 0x73;
    writeData(0x0302 + x, a);
    a = 0x03;
    writeData(0x0303 + x, a);
    y = M(WorldNumber);
    ++y;
    a = y;
    writeData(0x0304 + x, a);
    a = 0x28;
    writeData(0x0305 + x, a);
    y = M(LevelNumber);
    ++y;
    a = y;
    writeData(0x0306 + x, a);
    a = 0x00;
    writeData(0x0307 + x, a);
    a = x;
    c = 0;
    a += 0x06;
    writeData(VRAM_Buffer1_Offset, a);
    goto IncSubtask;
    // ;-------------------------------------------------------------------------------------

DisplayTimeUp:
    a = M(GameTimerExpiredFlag);
    if (z)
        goto NoTimeUp;
    a = 0x00;
    writeData(GameTimerExpiredFlag, a);
    a = 0x02;
    goto OutputInter;

NoTimeUp:
    ++M(ScreenRoutineTask);
    goto IncSubtask;
    // ;-------------------------------------------------------------------------------------

DisplayIntermediate:
    a = M(OperMode);
    if (z)
        goto NoInter;
    compare(a, 0x03);
    if (z)
        goto GameOverInter;
    a = M(AltEntranceControl);
    if (!z)
        goto NoInter;
    y = M(AreaType);
    compare(y, 0x03);
    if (z)
        goto PlayerInter;
    a = M(DisableIntermediate);
    if (!z)
        goto NoInter;

PlayerInter:
    JSR(DrawPlayer_Intermediate, 34);
    a = 0x01;

OutputInter:
    JSR(WriteGameText, 35);
    JSR(ResetScreenTimer, 36);
    a = 0x00;
    writeData(DisableScreenFlag, a);
    goto Return;

GameOverInter:
    a = 0x12;
    writeData(ScreenTimer, a);
    a = 0x03;
    JSR(WriteGameText, 37);
    goto IncModeTask_B;

NoInter:
    a = 0x08;
    writeData(ScreenRoutineTask, a);
    goto Return;
    // ;-------------------------------------------------------------------------------------

AreaParserTaskControl:
    ++M(DisableScreenFlag);

TaskLoop:
    JSR(AreaParserTaskHandler, 38);
    a = M(AreaParserTaskNum);
    if (!z)
        goto TaskLoop;
    --M(ColumnSets);
    if (!n)
        goto OutputCol;
    ++M(ScreenRoutineTask);

OutputCol:
    a = 0x06;
    writeData(VRAM_Buffer_AddrCtrl, a);
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$00 - vram buffer address table low
    // ;$01 - vram buffer address table high

DrawTitleScreen:
    a = M(OperMode);
    if (!z)
        goto IncModeTask_B;
    a = 0x1e;
    writeData(PPU_ADDRESS, a);
    a = 0xc0;
    writeData(PPU_ADDRESS, a);
    a = 0x03;
    writeData(GameModeValue, a);
    y = 0x00;
    writeData(TitleScreenModeValue, y);
    a = M(PPU_DATA);

OutputTScr:
    a = M(PPU_DATA);
    writeData(W(TitleScreenModeValue) + y, a);
    ++y;
    if (!z)
        goto ChkHiByte;
    ++M(GameModeValue);

ChkHiByte:
    a = M(GameModeValue);
    compare(a, 0x04);
    if (!z)
        goto OutputTScr;
    compare(y, 0x3a);
    if (!c)
        goto OutputTScr;
    a = 0x05;
    goto SetVRAMAddr_B;
    // ;-------------------------------------------------------------------------------------

ClearBuffersDrawIcon:
    a = M(OperMode);
    if (!z)
        goto IncModeTask_B;
    x = 0x00;

TScrClear:
    writeData(VRAM_Buffer1_Offset + x, a);
    writeData(SprObject_X_MoveForce + x, a);
    --x;
    if (!z)
        goto TScrClear;
    JSR(DrawMushroomIcon, 39);

IncSubtask:
    ++M(ScreenRoutineTask);
    goto Return;
    // ;-------------------------------------------------------------------------------------

WriteTopScore:
    a = 0xfa;
    JSR(UpdateNumber, 40);

IncModeTask_B:
    ++M(OperMode_Task);
    goto Return;
    // ;-------------------------------------------------------------------------------------

GameText:

WriteGameText:
    pha();
    a <<= 1;
    y = a;
    compare(y, 0x04);
    if (!c)
        goto LdGameText;
    compare(y, 0x08);
    if (!c)
        goto Chk2Players;
    y = 0x08;

Chk2Players:
    a = M(NumberOfPlayers);
    if (!z)
        goto LdGameText;
    ++y;

LdGameText:
    x = M(GameTextOffsets + y);
    y = 0x00;

GameTextLoop:
    a = M(TopStatusBarLine + x);
    compare(a, 0xff);
    if (z)
        goto EndGameText;
    writeData(VRAM_Buffer1 + y, a);
    ++x;
    ++y;
    if (!z)
        goto GameTextLoop;

EndGameText:
    a = 0x00;
    writeData(VRAM_Buffer1 + y, a);
    pla();
    x = a;
    compare(a, 0x04);
    if (c)
        goto PrintWarpZoneNumbers;
    --x;
    if (!z)
        goto CheckPlayerName;
    a = M(NumberofLives);
    c = 0;
    a += 0x01;
    compare(a, 0x0a);
    if (!c)
        goto PutLives;
    a -= 0x0a;
    y = 0x9f;
    writeData(0x0308, y);

PutLives:
    writeData(0x0309, a);
    y = M(WorldNumber);
    ++y;
    writeData(0x0314, y);
    y = M(LevelNumber);
    ++y;
    writeData(0x0316, y);
    goto Return;

CheckPlayerName:
    a = M(NumberOfPlayers);
    if (z)
        goto ExitChkName;
    a = M(CurrentPlayer);
    --x;
    if (!z)
        goto ChkLuigi;
    y = M(OperMode);
    compare(y, 0x03);
    if (z)
        goto ChkLuigi;
    a ^= 0x01;

ChkLuigi:
    a >>= 1;
    if (!c)
        goto ExitChkName;
    y = 0x04;

NameLoop:
    a = M(LuigiName + y);
    writeData(0x0304 + y, a);
    --y;
    if (!n)
        goto NameLoop;

ExitChkName:
    goto Return;

PrintWarpZoneNumbers:
    a -= 0x04;
    a <<= 1;
    a <<= 1;
    x = a;
    y = 0x00;

WarpNumLoop:
    a = M(WarpZoneNumbers + x);
    writeData(0x031c + y, a);
    ++x;
    ++y;
    ++y;
    ++y;
    ++y;
    compare(y, 0x0c);
    if (!c)
        goto WarpNumLoop;
    a = 0x2c;
    goto SetVRAMOffset;
    // ;-------------------------------------------------------------------------------------

ResetSpritesAndScreenTimer:
    a = M(ScreenTimer);
    if (!z)
        goto NoReset;
    JSR(MoveAllSpritesOffscreen, 41);

ResetScreenTimer:
    a = 0x07;
    writeData(ScreenTimer, a);
    ++M(ScreenRoutineTask);

NoReset:
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$00 - temp vram buffer offset
    // ;$01 - temp metatile buffer offset
    // ;$02 - temp metatile graphics table offset
    // ;$03 - used to store attribute bits
    // ;$04 - used to determine attribute table row
    // ;$05 - used to determine attribute table column
    // ;$06 - metatile graphics table address low
    // ;$07 - metatile graphics table address high

RenderAreaGraphics:
    a = M(CurrentColumnPos);
    a &= 0x01;
    writeData(World6, a);
    y = M(VRAM_Buffer2_Offset);
    writeData(TitleScreenModeValue, y);
    a = M(CurrentNTAddr_Low);
    writeData(0x0342 + y, a);
    a = M(CurrentNTAddr_High);
    writeData(VRAM_Buffer2 + y, a);
    a = 0x9a;
    writeData(0x0343 + y, a);
    a = 0x00;
    writeData(World5, a);
    x = a;

DrawMTLoop:
    writeData(GameModeValue, x);
    a = M(MetatileBuffer + x);
    a &= 0xc0;
    writeData(GameOverModeValue, a);
    a <<= 1;
    a.rol();
    a.rol();
    y = a;
    a = M(MetatileGraphics_Low + y);
    writeData(World7, a);
    a = M(MetatileGraphics_High + y);
    writeData(World8, a);
    a = M(MetatileBuffer + x);
    a <<= 1;
    a <<= 1;
    writeData(VictoryModeValue, a);
    a = M(AreaParserTaskNum);
    a &= 0x01;
    a ^= 0x01;
    a <<= 1;
    a += M(VictoryModeValue);
    y = a;
    x = M(TitleScreenModeValue);
    a = M(W(World7) + y);
    writeData(0x0344 + x, a);
    ++y;
    a = M(W(World7) + y);
    writeData(0x0345 + x, a);
    y = M(World5);
    a = M(World6);
    if (!z)
        goto RightCheck;
    a = M(GameModeValue);
    a >>= 1;
    if (c)
        goto LLeft;
    M(GameOverModeValue).rol();
    M(GameOverModeValue).rol();
    M(GameOverModeValue).rol();
    goto SetAttrib;

RightCheck:
    a = M(GameModeValue);
    a >>= 1;
    if (c)
        goto NextMTRow;
    M(GameOverModeValue) >>= 1;
    M(GameOverModeValue) >>= 1;
    M(GameOverModeValue) >>= 1;
    M(GameOverModeValue) >>= 1;
    goto SetAttrib;

LLeft:
    M(GameOverModeValue) >>= 1;
    M(GameOverModeValue) >>= 1;

NextMTRow:
    ++M(World5);

SetAttrib:
    a = M(AttributeBuffer + y);
    a |= M(GameOverModeValue);
    writeData(AttributeBuffer + y, a);
    ++M(TitleScreenModeValue);
    ++M(TitleScreenModeValue);
    x = M(GameModeValue);
    ++x;
    compare(x, 0x0d);
    if (!c)
        goto DrawMTLoop;
    y = M(TitleScreenModeValue);
    ++y;
    ++y;
    ++y;
    a = 0x00;
    writeData(VRAM_Buffer2 + y, a);
    writeData(VRAM_Buffer2_Offset, y);
    ++M(CurrentNTAddr_Low);
    a = M(CurrentNTAddr_Low);
    a &= 0x1f;
    if (!z)
        goto ExitDrawM;
    a = 0x80;
    writeData(CurrentNTAddr_Low, a);
    a = M(CurrentNTAddr_High);
    a ^= 0x04;
    writeData(CurrentNTAddr_High, a);

ExitDrawM:
    goto SetVRAMCtrl;
    // ;-------------------------------------------------------------------------------------
    // ;$00 - temp attribute table address high (big endian order this time!)
    // ;$01 - temp attribute table address low

RenderAttributeTables:
    a = M(CurrentNTAddr_Low);
    a &= 0x1f;
    c = 1;
    a -= 0x04;
    a &= 0x1f;
    writeData(GameModeValue, a);
    a = M(CurrentNTAddr_High);
    if (c)
        goto SetATHigh;
    a ^= 0x04;

SetATHigh:
    a &= 0x04;
    a |= 0x23;
    writeData(TitleScreenModeValue, a);
    a = M(GameModeValue);
    a >>= 1;
    a >>= 1;
    a += 0xc0;
    writeData(GameModeValue, a);
    x = 0x00;
    y = M(VRAM_Buffer2_Offset);

AttribLoop:
    a = M(TitleScreenModeValue);
    writeData(VRAM_Buffer2 + y, a);
    a = M(GameModeValue);
    c = 0;
    a += 0x08;
    writeData(0x0342 + y, a);
    writeData(GameModeValue, a);
    a = M(AttributeBuffer + x);
    writeData(0x0344 + y, a);
    a = 0x01;
    writeData(0x0343 + y, a);
    a >>= 1;
    writeData(AttributeBuffer + x, a);
    ++y;
    ++y;
    ++y;
    ++y;
    ++x;
    compare(x, 0x07);
    if (!c)
        goto AttribLoop;
    writeData(VRAM_Buffer2 + y, a);
    writeData(VRAM_Buffer2_Offset, y);

SetVRAMCtrl:
    a = 0x06;
    writeData(VRAM_Buffer_AddrCtrl, a);
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$00 - used as temporary counter in ColorRotation
    // ;used based on area type

ColorRotation:
    a = M(TallEnemy);
    a &= 0x07;
    if (!z)
        goto ExitColorRot;
    x = M(VRAM_Buffer1_Offset);
    compare(x, 0x31);
    if (c)
        goto ExitColorRot;
    y = a;

GetBlankPal:
    a = M(BlankPalette + y);
    writeData(VRAM_Buffer1 + x, a);
    ++x;
    ++y;
    compare(y, 0x08);
    if (!c)
        goto GetBlankPal;
    x = M(VRAM_Buffer1_Offset);
    a = 0x03;
    writeData(TitleScreenModeValue, a);
    a = M(AreaType);
    a <<= 1;
    a <<= 1;
    y = a;

GetAreaPal:
    a = M(Palette3Data + y);
    writeData(0x0304 + x, a);
    ++y;
    ++x;
    --M(TitleScreenModeValue);
    if (!n)
        goto GetAreaPal;
    x = M(VRAM_Buffer1_Offset);
    y = M(ColorRotateOffset);
    a = M(ColorRotatePalette + y);
    writeData(0x0305 + x, a);
    a = M(VRAM_Buffer1_Offset);
    c = 0;
    a += 0x07;
    writeData(VRAM_Buffer1_Offset, a);
    ++M(ColorRotateOffset);
    a = M(ColorRotateOffset);
    compare(a, 0x06);
    if (!c)
        goto ExitColorRot;
    a = 0x00;
    writeData(ColorRotateOffset, a);

ExitColorRot:
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$00 - temp store for offset control bit
    // ;$01 - temp vram buffer offset
    // ;$02 - temp store for vertical high nybble in block buffer routine
    // ;$03 - temp adder for high byte of name table address
    // ;$04, $05 - name table address low/high
    // ;$06, $07 - block buffer address low/high

RemoveCoin_Axe:
    y = 0x41;
    a = 0x03;
    x = M(AreaType);
    if (!z)
        goto WriteBlankMT;
    a = 0x04;

WriteBlankMT:
    JSR(PutBlockMetatile, 42);
    a = 0x06;
    writeData(VRAM_Buffer_AddrCtrl, a);
    goto Return;

ReplaceBlockMetatile:
    JSR(WriteBlockMetatile, 43);
    ++M(Block_ResidualCounter);
    --M(Block_RepFlag + x);
    goto Return;

DestroyBlockMetatile:
    a = 0x00;

WriteBlockMetatile:
    y = 0x03;
    compare(a, 0x00);
    if (z)
        goto UseBOffset;
    y = 0x00;
    compare(a, 0x58);
    if (z)
        goto UseBOffset;
    compare(a, 0x51);
    if (z)
        goto UseBOffset;
    ++y;
    compare(a, 0x5d);
    if (z)
        goto UseBOffset;
    compare(a, 0x52);
    if (z)
        goto UseBOffset;
    ++y;

UseBOffset:
    a = y;
    y = M(VRAM_Buffer1_Offset);
    ++y;
    JSR(PutBlockMetatile, 44);

MoveVOffset:
    --y;
    a = y;
    c = 0;
    a += 0x0a;
    goto SetVRAMOffset;

PutBlockMetatile:
    writeData(TitleScreenModeValue, x);
    writeData(GameModeValue, y);
    a <<= 1;
    a <<= 1;
    x = a;
    y = 0x20;
    a = M(World7);
    compare(a, 0xd0);
    if (!c)
        goto SaveHAdder;
    y = 0x24;

SaveHAdder:
    writeData(GameOverModeValue, y);
    a &= 0x0f;
    a <<= 1;
    writeData(World5, a);
    a = 0x00;
    writeData(World6, a);
    a = M(VictoryModeValue);
    c = 0;
    a += 0x20;
    a <<= 1;
    M(World6).rol();
    a <<= 1;
    M(World6).rol();
    a += M(World5);
    writeData(World5, a);
    a = M(World6);
    a += 0x00;
    c = 0;
    a += M(GameOverModeValue);
    writeData(World6, a);
    y = M(GameModeValue);

RemBridge:
    a = M(BlockGfxData + x);
    writeData(0x0303 + y, a);
    a = M(0x8a3a + x);
    writeData(0x0304 + y, a);
    a = M(0x8a3b + x);
    writeData(0x0308 + y, a);
    a = M(0x8a3c + x);
    writeData(0x0309 + y, a);
    a = M(World5);
    writeData(VRAM_Buffer1 + y, a);
    c = 0;
    a += 0x20;
    writeData(0x0306 + y, a);
    a = M(World6);
    writeData(VRAM_Buffer1_Offset + y, a);
    writeData(0x0305 + y, a);
    a = 0x02;
    writeData(0x0302 + y, a);
    writeData(0x0307 + y, a);
    a = 0x00;
    writeData(0x030a + y, a);
    x = M(TitleScreenModeValue);
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;METATILE GRAPHICS TABLE
    // ;-------------------------------------------------------------------------------------
    // ;VRAM BUFFER DATA FOR LOCATIONS IN PRG-ROM
    // ;"THANK YOU MARIO!"
    // ;"THANK YOU LUIGI!"
    // ;"BUT OUR PRINCESS IS IN"
    // ;"ANOTHER CASTLE!"
    // ;"YOUR QUEST IS OVER."
    // ;"WE PRESENT YOU A NEW QUEST."
    // ;"PUSH BUTTON B"
    // ;"TO SELECT A WORLD"
    // ;-------------------------------------------------------------------------------------
    // ;$04 - address low to jump address
    // ;$05 - address high to jump address
    // ;$06 - jump address low
    // ;$07 - jump address high

JumpEngine:
    a <<= 1;
    y = a;
    pla();
    writeData(World5, a);
    pla();
    writeData(World6, a);
    ++y;
    a = M(W(World5) + y);
    writeData(World7, a);
    ++y;
    a = M(W(World5) + y);
    writeData(World8, a);
    // goto World7; // Invalid jump to constant/indirect
    // ;-------------------------------------------------------------------------------------

InitializeNameTables:
    a = M(PPU_STATUS);
    a = M(Mirror_PPU_CTRL_REG1);
    a |= 0x10;
    a &= 0xf0;
    JSR(WritePPUReg1, 45);
    a = 0x24;
    JSR(WriteNTAddr, 46);
    a = 0x20;

WriteNTAddr:
    writeData(PPU_ADDRESS, a);
    a = 0x00;
    writeData(PPU_ADDRESS, a);
    x = 0x04;
    y = 0xc0;
    a = 0x24;

InitNTLoop:
    writeData(PPU_DATA, a);
    --y;
    if (!z)
        goto InitNTLoop;
    --x;
    if (!z)
        goto InitNTLoop;
    y = 0x40;
    a = x;
    writeData(VRAM_Buffer1_Offset, a);
    writeData(VRAM_Buffer1, a);

InitATLoop:
    writeData(PPU_DATA, a);
    --y;
    if (!z)
        goto InitATLoop;
    writeData(HorizontalScroll, a);
    writeData(VerticalScroll, a);
    goto InitScroll;
    // ;-------------------------------------------------------------------------------------
    // ;$00 - temp joypad bit

ReadJoypads:
    a = 0x01;
    writeData(JOYPAD_PORT1, a);
    a >>= 1;
    x = a;
    writeData(JOYPAD_PORT1, a);
    JSR(ReadPortBits, 47);
    ++x;

ReadPortBits:
    y = 0x08;

PortLoop:
    pha();
    a = M(JOYPAD_PORT1 + x);
    writeData(TitleScreenModeValue, a);
    a >>= 1;
    a |= M(TitleScreenModeValue);
    a >>= 1;
    pla();
    a.rol();
    --y;
    if (!z)
        goto PortLoop;
    writeData(SavedJoypad1Bits + x, a);
    pha();
    a &= 0x30;
    a &= M(JoypadBitMask + x);
    if (z)
        goto Save8Bits;
    pla();
    a &= 0xcf;
    writeData(SavedJoypad1Bits + x, a);
    goto Return;

Save8Bits:
    pla();
    writeData(JoypadBitMask + x, a);
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$00 - vram buffer address table low
    // ;$01 - vram buffer address table high

WriteBufferToScreen:
    writeData(PPU_ADDRESS, a);
    ++y;
    a = M(W(TitleScreenModeValue) + y);
    writeData(PPU_ADDRESS, a);
    ++y;
    a = M(W(TitleScreenModeValue) + y);
    a <<= 1;
    pha();
    a = M(Mirror_PPU_CTRL_REG1);
    a |= 0x04;
    if (c)
        goto SetupWrites;
    a &= 0xfb;

SetupWrites:
    JSR(WritePPUReg1, 48);
    pla();
    a <<= 1;
    if (!c)
        goto GetLength;
    a |= 0x02;
    ++y;

GetLength:
    a >>= 1;
    a >>= 1;
    x = a;

OutputToVRAM:
    if (c)
        goto RepeatByte;
    ++y;

RepeatByte:
    a = M(W(TitleScreenModeValue) + y);
    writeData(PPU_DATA, a);
    --x;
    if (!z)
        goto OutputToVRAM;
    c = 1;
    a = y;
    a += M(TitleScreenModeValue);
    writeData(TitleScreenModeValue, a);
    a = 0x00;
    a += M(GameModeValue);
    writeData(GameModeValue, a);
    a = 0x3f;
    writeData(PPU_ADDRESS, a);
    a = 0x00;
    writeData(PPU_ADDRESS, a);
    writeData(PPU_ADDRESS, a);
    writeData(PPU_ADDRESS, a);

UpdateScreen:
    x = M(PPU_STATUS);
    y = 0x00;
    a = M(W(TitleScreenModeValue) + y);
    if (!z)
        goto WriteBufferToScreen;

InitScroll:
    writeData(PPU_SCROLL_REG, a);
    writeData(PPU_SCROLL_REG, a);
    goto Return;
    // ;-------------------------------------------------------------------------------------

WritePPUReg1:
    writeData(PPU_CTRL_REG1, a);
    writeData(Mirror_PPU_CTRL_REG1, a);
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$00 - used to store status bar nybbles
    // ;$02 - used as temp vram offset
    // ;$03 - used to store length of status bar number
    // ;status bar name table offset and length data

PrintStatusBarNumbers:
    writeData(TitleScreenModeValue, a);
    JSR(OutputNumbers, 49);
    a = M(TitleScreenModeValue);
    a >>= 1;
    a >>= 1;
    a >>= 1;
    a >>= 1;

OutputNumbers:
    c = 0;
    a += 0x01;
    a &= 0x0f;
    compare(a, 0x06);
    if (c)
        goto ExitOutputN;
    pha();
    a <<= 1;
    y = a;
    x = M(VRAM_Buffer1_Offset);
    a = 0x20;
    compare(y, 0x00);
    if (!z)
        goto SetupNums;
    a = 0x22;

SetupNums:
    writeData(VRAM_Buffer1 + x, a);
    a = M(StatusBarData + y);
    writeData(0x0302 + x, a);
    a = M(0x8ef5 + y);
    writeData(0x0303 + x, a);
    writeData(GameOverModeValue, a);
    writeData(VictoryModeValue, x);
    pla();
    x = a;
    a = M(StatusBarOffset + x);
    c = 1;
    a -= M(0x8ef5 + y);
    y = a;
    x = M(VictoryModeValue);

DigitPLoop:
    a = M(TopScoreDisplay + y);
    writeData(0x0304 + x, a);
    ++x;
    ++y;
    --M(GameOverModeValue);
    if (!z)
        goto DigitPLoop;
    a = 0x00;
    writeData(0x0304 + x, a);
    ++x;
    ++x;
    ++x;
    writeData(VRAM_Buffer1_Offset, x);

ExitOutputN:
    goto Return;
    // ;-------------------------------------------------------------------------------------

DigitsMathRoutine:
    a = M(OperMode);
    compare(a, 0x00);
    if (z)
        goto EraseDMods;
    x = 0x05;

AddModLoop:
    a = M(DigitModifier + x);
    c = 0;
    a += M(TopScoreDisplay + y);
    if (n)
        goto BorrowOne;
    compare(a, 0x0a);
    if (c)
        goto CarryOne;

StoreNewD:
    writeData(TopScoreDisplay + y, a);
    --y;
    --x;
    if (!n)
        goto AddModLoop;

EraseDMods:
    a = 0x00;
    x = 0x06;

EraseMLoop:
    writeData(0x0133 + x, a);
    --x;
    if (!n)
        goto EraseMLoop;
    goto Return;

BorrowOne:
    --M(0x0133 + x);
    a = 0x09;
    if (!z)
        goto StoreNewD;

CarryOne:
    c = 1;
    a -= 0x0a;
    ++M(0x0133 + x);
    goto StoreNewD;
    // ;-------------------------------------------------------------------------------------

UpdateTopScore:
    x = 0x05;
    JSR(TopScoreCheck, 50);
    x = 0x0b;

TopScoreCheck:
    y = 0x05;
    c = 1;

GetScoreDiff:
    a = M(PlayerScoreDisplay + x);
    a -= M(TopScoreDisplay + y);
    --x;
    --y;
    if (!n)
        goto GetScoreDiff;
    if (!c)
        goto NoTopSc;
    ++x;
    ++y;

CopyScore:
    a = M(PlayerScoreDisplay + x);
    writeData(TopScoreDisplay + y, a);
    ++x;
    ++y;
    compare(y, 0x06);
    if (!c)
        goto CopyScore;

NoTopSc:
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;-------------------------------------------------------------------------------------

InitializeGame:
    y = 0x6f;
    JSR(InitializeMemory, 51);
    y = 0x1f;

ClrSndLoop:
    writeData(SoundMemory + y, a);
    --y;
    if (!n)
        goto ClrSndLoop;
    a = 0x18;
    writeData(DemoTimer, a);
    JSR(LoadAreaPointer, 52);

InitializeArea:
    y = 0x4b;
    JSR(InitializeMemory, 53);
    x = 0x21;
    a = 0x00;

ClrTimersLoop:
    writeData(SelectTimer + x, a);
    --x;
    if (!n)
        goto ClrTimersLoop;
    a = M(HalfwayPage);
    y = M(AltEntranceControl);
    if (z)
        goto StartPage;
    a = M(EntrancePage);

StartPage:
    writeData(ScreenLeft_PageLoc, a);
    writeData(CurrentPageLoc, a);
    writeData(BackloadingFlag, a);
    JSR(GetScreenPosition, 54);
    y = 0x20;
    a &= 0x01;
    if (z)
        goto SetInitNTHigh;
    y = 0x24;

SetInitNTHigh:
    writeData(CurrentNTAddr_High, y);
    y = 0x80;
    writeData(CurrentNTAddr_Low, y);
    a <<= 1;
    a <<= 1;
    a <<= 1;
    a <<= 1;
    writeData(BlockBufferColumnPos, a);
    --M(AreaObjectLength);
    --M(0x0731);
    --M(0x0732);
    a = 0x0b;
    writeData(ColumnSets, a);
    JSR(GetAreaDataAddrs, 55);
    a = M(PrimaryHardMode);
    if (!z)
        goto SetSecHard;
    a = M(WorldNumber);
    compare(a, 0x04);
    if (!c)
        goto CheckHalfway;
    if (!z)
        goto SetSecHard;
    a = M(LevelNumber);
    compare(a, 0x02);
    if (!c)
        goto CheckHalfway;

SetSecHard:
    ++M(SecondaryHardMode);

CheckHalfway:
    a = M(HalfwayPage);
    if (z)
        goto DoneInitArea;
    a = 0x02;
    writeData(PlayerEntranceCtrl, a);

DoneInitArea:
    a = 0x80;
    writeData(AreaMusicQueue, a);
    a = 0x01;
    writeData(DisableScreenFlag, a);
    ++M(OperMode_Task);
    goto Return;
    // ;-------------------------------------------------------------------------------------

PrimaryGameSetup:
    a = 0x01;
    writeData(FetchNewGameTimerFlag, a);
    writeData(PlayerSize, a);
    a = 0x02;
    writeData(NumberofLives, a);
    writeData(OffScr_NumberofLives, a);

SecondaryGameSetup:
    a = 0x00;
    writeData(DisableScreenFlag, a);
    y = a;

ClearVRLoop:
    writeData(VRAM_Buffer1_Offset + y, a);
    ++y;
    if (!z)
        goto ClearVRLoop;
    writeData(GameTimerExpiredFlag, a);
    writeData(DisableIntermediate, a);
    writeData(BackloadingFlag, a);
    a = 0xff;
    writeData(BalPlatformAlignment, a);
    a = M(ScreenLeft_PageLoc);
    M(Mirror_PPU_CTRL_REG1) >>= 1;
    a &= 0x01;
    a.ror();
    M(Mirror_PPU_CTRL_REG1).rol();
    JSR(GetAreaMusic, 56);
    a = 0x38;
    writeData(0x06e3, a);
    a = 0x48;
    writeData(0x06e2, a);
    a = 0x58;
    writeData(SprShuffleAmt, a);
    x = 0x0e;

ShufAmtLoop:
    a = M(DefaultSprOffsets + x);
    writeData(Player_SprDataOffset + x, a);
    --x;
    if (!n)
        goto ShufAmtLoop;
    y = 0x03;

ISpr0Loop:
    a = M(Sprite0Data + y);
    writeData(Sprite_Y_Position + y, a);
    --y;
    if (!n)
        goto ISpr0Loop;
    JSR(DoNothing2, 57);
    JSR(DoNothing1, 58);
    ++M(Sprite0HitDetectFlag);
    ++M(OperMode_Task);
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$06 - RAM address low
    // ;$07 - RAM address high

InitializeMemory:
    x = 0x07;
    a = 0x00;
    writeData(World7, a);

InitPageLoop:
    writeData(World8, x);

InitByteLoop:
    compare(x, 0x01);
    if (!z)
        goto InitByte;
    compare(y, 0x60);
    if (c)
        goto SkipByte;

InitByte:
    writeData(W(World7) + y, a);

SkipByte:
    --y;
    compare(y, 0xff);
    if (!z)
        goto InitByteLoop;
    --x;
    if (!n)
        goto InitPageLoop;
    goto Return;
    // ;-------------------------------------------------------------------------------------

GetAreaMusic:
    a = M(OperMode);
    if (z)
        goto ExitGetM;
    a = M(AltEntranceControl);
    compare(a, 0x02);
    if (z)
        goto ChkAreaType;
    y = 0x05;
    a = M(PlayerEntranceCtrl);
    compare(a, 0x06);
    if (z)
        goto StoreMusic;
    compare(a, 0x07);
    if (z)
        goto StoreMusic;

ChkAreaType:
    y = M(AreaType);
    a = M(CloudTypeOverride);
    if (z)
        goto StoreMusic;
    y = 0x04;

StoreMusic:
    a = M(MusicSelectData + y);
    writeData(AreaMusicQueue, a);

ExitGetM:
    goto Return;
    // ;-------------------------------------------------------------------------------------

Entrance_GameTimerSetup:
    a = M(ScreenLeft_PageLoc);
    writeData(Player_PageLoc, a);
    a = 0x28;
    writeData(VerticalForceDown, a);
    a = 0x01;
    writeData(BulletBill_CannonVar, a);
    writeData(Player_Y_HighPos, a);
    a = 0x00;
    writeData(Player_State, a);
    --M(Player_CollisionBits);
    y = 0x00;
    writeData(HalfwayPage, y);
    a = M(AreaType);
    if (!z)
        goto ChkStPos;
    ++y;

ChkStPos:
    writeData(SwimmingFlag, y);
    x = M(PlayerEntranceCtrl);
    y = M(AltEntranceControl);
    if (z)
        goto SetStPos;
    compare(y, 0x01);
    if (z)
        goto SetStPos;
    x = M(data_9118 + y);

SetStPos:
    a = M(PlayerStarting_X_Pos + y);
    writeData(Player_X_Position, a);
    a = M(PlayerStarting_Y_Pos + x);
    writeData(Player_Y_Position, a);
    a = M(PlayerBGPriorityData + x);
    writeData(Player_SprAttrib, a);
    JSR(GetPlayerColors, 59);
    y = M(GameTimerSetting);
    if (z)
        goto ChkOverR;
    a = M(FetchNewGameTimerFlag);
    if (z)
        goto ChkOverR;
    a = M(GameTimerData + y);
    writeData(GameTimerDisplay, a);
    a = 0x01;
    writeData(0x07fa, a);
    a >>= 1;
    writeData(0x07f9, a);
    writeData(FetchNewGameTimerFlag, a);
    writeData(StarInvincibleTimer, a);

ChkOverR:
    y = M(JoypadOverride);
    if (z)
        goto ChkSwimE;
    a = 0x03;
    writeData(Player_State, a);
    x = 0x00;
    JSR(InitBlock_XY_Pos, 60);
    a = 0xf0;
    writeData(Block_Y_Position, a);
    x = 0x05;
    y = 0x00;
    JSR(Setup_Vine, 61);

ChkSwimE:
    y = M(AreaType);
    if (!z)
        goto SetPESub;
    JSR(SetupBubble, 62);

SetPESub:
    a = 0x07;
    writeData(GreenParatroopaJump, a);
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;page numbers are in order from -1 to -4

PlayerLoseLife:
    ++M(DisableScreenFlag);
    a = 0x00;
    writeData(Sprite0HitDetectFlag, a);
    a = 0x80;
    writeData(EventMusicQueue, a);
    --M(NumberofLives);
    if (!n)
        goto StillInGame;
    a = 0x00;
    writeData(OperMode_Task, a);
    a = 0x03;
    writeData(OperMode, a);
    goto Return;

StillInGame:
    a = M(WorldNumber);
    a <<= 1;
    x = a;
    a = M(LevelNumber);
    a &= 0x02;
    if (z)
        goto GetHalfway;
    ++x;

GetHalfway:
    y = M(HalfwayPageNybbles + x);
    a = M(LevelNumber);
    a >>= 1;
    a = y;
    if (c)
        goto MaskHPNyb;
    a >>= 1;
    a >>= 1;
    a >>= 1;
    a >>= 1;

MaskHPNyb:
    a &= 0x0f;
    compare(a, M(ScreenLeft_PageLoc));
    if (z)
        goto SetHalfway;
    if (!c)
        goto SetHalfway;
    a = 0x00;

SetHalfway:
    writeData(HalfwayPage, a);
    JSR(TransposePlayers, 63);
    goto ContinueGame;
    // ;-------------------------------------------------------------------------------------

GameOverMode:
    a = M(OperMode_Task);
    switch (a) {
        case 0: goto SetupGameOver;
        case 1: goto ScreenRoutines;
        case 2: goto RunGameOver;
    }

SetupGameOver:
    a = 0x00;
    writeData(ScreenRoutineTask, a);
    writeData(Sprite0HitDetectFlag, a);
    a = 0x02;
    writeData(EventMusicQueue, a);
    ++M(DisableScreenFlag);
    ++M(OperMode_Task);
    goto Return;
    // ;-------------------------------------------------------------------------------------

RunGameOver:
    a = 0x00;
    writeData(DisableScreenFlag, a);
    a = M(SavedJoypad1Bits);
    a &= 0x10;
    if (!z)
        goto TerminateGame;
    a = M(ScreenTimer);
    if (!z)
        goto GameIsOn;

TerminateGame:
    a = 0x80;
    writeData(EventMusicQueue, a);
    JSR(TransposePlayers, 64);
    if (!c)
        goto ContinueGame;
    a = M(WorldNumber);
    writeData(ContinueWorld, a);
    a = 0x00;
    a <<= 1;
    writeData(OperMode_Task, a);
    writeData(ScreenTimer, a);
    writeData(OperMode, a);
    goto Return;

ContinueGame:
    JSR(LoadAreaPointer, 65);
    a = 0x01;
    writeData(PlayerSize, a);
    ++M(FetchNewGameTimerFlag);
    a = 0x00;
    writeData(TimerControl, a);
    writeData(PlayerStatus, a);
    writeData(GreenParatroopaJump, a);
    writeData(OperMode_Task, a);
    a = 0x01;
    writeData(OperMode, a);

GameIsOn:
    goto Return;

TransposePlayers:
    c = 1;
    a = M(NumberOfPlayers);
    if (z)
        goto ExTrans;
    a = M(OffScr_NumberofLives);
    if (n)
        goto ExTrans;
    a = M(CurrentPlayer);
    a ^= 0x01;
    writeData(CurrentPlayer, a);
    x = 0x06;

TransLoop:
    a = M(NumberofLives + x);
    pha();
    a = M(OffScr_NumberofLives + x);
    writeData(NumberofLives + x, a);
    pla();
    writeData(OffScr_NumberofLives + x, a);
    --x;
    if (!n)
        goto TransLoop;
    c = 0;

ExTrans:
    goto Return;
    // ;-------------------------------------------------------------------------------------

DoNothing1:
    a = 0xff;
    writeData(0x06c9, a);

DoNothing2:
    goto Return;
    // ;-------------------------------------------------------------------------------------

AreaParserTaskHandler:
    y = M(AreaParserTaskNum);
    if (!z)
        goto DoAPTasks;
    y = 0x08;
    writeData(AreaParserTaskNum, y);

DoAPTasks:
    --y;
    a = y;
    JSR(AreaParserTasks, 66);
    --M(AreaParserTaskNum);
    if (!z)
        goto SkipATRender;
    JSR(RenderAttributeTables, 67);

SkipATRender:
    goto Return;

AreaParserTasks:
    switch (a) {
        case 0: goto IncrementColumnPos;
        case 1: goto RenderAreaGraphics;
        case 2: goto RenderAreaGraphics;
        case 3: goto AreaParserCore;
        case 4: goto IncrementColumnPos;
        case 5: goto RenderAreaGraphics;
        case 6: goto RenderAreaGraphics;
        case 7: goto AreaParserCore;
    }

IncrementColumnPos:
    ++M(CurrentColumnPos);
    a = M(CurrentColumnPos);
    a &= 0x0f;
    if (!z)
        goto NoColWrap;
    writeData(CurrentColumnPos, a);
    ++M(CurrentPageLoc);

NoColWrap:
    ++M(BlockBufferColumnPos);
    a = M(BlockBufferColumnPos);
    a &= 0x1f;
    writeData(BlockBufferColumnPos, a);
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$00 - used as counter, store for low nybble for background, ceiling byte for terrain
    // ;$01 - used to store floor byte for terrain
    // ;$07 - used to store terrain metatile
    // ;$06-$07 - used to store block buffer address

AreaParserCore:
    a = M(BackloadingFlag);
    if (z)
        goto RenderSceneryTerrain;
    JSR(ProcessAreaData, 68);

RenderSceneryTerrain:
    x = 0x0c;
    a = 0x00;

ClrMTBuf:
    writeData(MetatileBuffer + x, a);
    --x;
    if (!n)
        goto ClrMTBuf;
    y = M(BackgroundScenery);
    if (z)
        goto RendFore;
    a = M(CurrentPageLoc);

ThirdP:
    compare(a, 0x03);
    if (n)
        goto RendBack;
    c = 1;
    a -= 0x03;
    if (!n)
        goto ThirdP;

RendBack:
    a <<= 1;
    a <<= 1;
    a <<= 1;
    a <<= 1;
    a += M(0x92f6 + y);
    a += M(CurrentColumnPos);
    x = a;
    a = M(BackSceneryData + x);
    if (z)
        goto RendFore;
    pha();
    a &= 0x0f;
    c = 1;
    a -= 0x01;
    writeData(TitleScreenModeValue, a);
    a <<= 1;
    a += M(TitleScreenModeValue);
    x = a;
    pla();
    a >>= 1;
    a >>= 1;
    a >>= 1;
    a >>= 1;
    y = a;
    a = 0x03;
    writeData(TitleScreenModeValue, a);

SceLoop1:
    a = M(BackSceneryMetatiles + x);
    writeData(MetatileBuffer + y, a);
    ++x;
    ++y;
    compare(y, 0x0b);
    if (z)
        goto RendFore;
    --M(TitleScreenModeValue);
    if (!z)
        goto SceLoop1;

RendFore:
    x = M(ForegroundScenery);
    if (z)
        goto RendTerr;
    y = M(0x93ad + x);
    x = 0x00;

SceLoop2:
    a = M(ForeSceneryData + y);
    if (z)
        goto NoFore;
    writeData(MetatileBuffer + x, a);

NoFore:
    ++y;
    ++x;
    compare(x, 0x0d);
    if (!z)
        goto SceLoop2;

RendTerr:
    y = M(AreaType);
    if (!z)
        goto TerMTile;
    a = M(WorldNumber);
    compare(a, 0x07);
    if (!z)
        goto TerMTile;
    a = 0x62;
    goto StoreMT;

TerMTile:
    a = M(TerrainMetatiles + y);
    y = M(CloudTypeOverride);
    if (z)
        goto StoreMT;
    a = 0x88;

StoreMT:
    writeData(World8, a);
    x = 0x00;
    a = M(TerrainControl);
    a <<= 1;
    y = a;

TerrLoop:
    a = M(TerrainRenderBits + y);
    writeData(TitleScreenModeValue, a);
    ++y;
    writeData(GameModeValue, y);
    a = M(CloudTypeOverride);
    if (z)
        goto NoCloud2;
    compare(x, 0x00);
    if (z)
        goto NoCloud2;
    a = M(TitleScreenModeValue);
    a &= 0x08;
    writeData(TitleScreenModeValue, a);

NoCloud2:
    y = 0x00;

TerrBChk:
    a = M(Bitmasks + y);
    bit(M(TitleScreenModeValue));
    if (z)
        goto NextTBit;
    a = M(World8);
    writeData(MetatileBuffer + x, a);

NextTBit:
    ++x;
    compare(x, 0x0d);
    if (z)
        goto RendBBuf;
    a = M(AreaType);
    compare(a, 0x02);
    if (!z)
        goto EndUChk;
    compare(x, 0x0b);
    if (!z)
        goto EndUChk;
    a = 0x54;
    writeData(World8, a);

EndUChk:
    ++y;
    compare(y, 0x08);
    if (!z)
        goto TerrBChk;
    y = M(GameModeValue);
    if (!z)
        goto TerrLoop;

RendBBuf:
    JSR(ProcessAreaData, 69);
    a = M(BlockBufferColumnPos);
    JSR(GetBlockBufferAddr, 70);
    x = 0x00;
    y = 0x00;

ChkMTLow:
    writeData(TitleScreenModeValue, y);
    a = M(MetatileBuffer + x);
    a &= 0xc0;
    a <<= 1;
    a.rol();
    a.rol();
    y = a;
    a = M(MetatileBuffer + x);
    compare(a, M(BlockBuffLowBounds + y));
    if (c)
        goto StrBlock;
    a = 0x00;

StrBlock:
    y = M(TitleScreenModeValue);
    writeData(W(World7) + y, a);
    a = y;
    c = 0;
    a += 0x10;
    y = a;
    ++x;
    compare(x, 0x0d);
    if (!c)
        goto ChkMTLow;
    goto Return;
    // ;numbers lower than these with the same attribute bits
    // ;will not be stored in the block buffer
    // ;-------------------------------------------------------------------------------------
    // ;$00 - used to store area object identifier
    // ;$07 - used as adder to find proper area object code

ProcessAreaData:
    x = 0x02;

ProcADLoop:
    writeData(BulletBill_FrenzyVar, x);
    a = 0x00;
    writeData(BehindAreaParserFlag, a);
    y = M(AreaDataOffset);
    a = M(W(AreaDataLow) + y);
    compare(a, 0xfd);
    if (z)
        goto RdyDecode;
    a = M(AreaObjectLength + x);
    if (!n)
        goto RdyDecode;
    ++y;
    a = M(W(AreaDataLow) + y);
    a <<= 1;
    if (!c)
        goto Chk1Row13;
    a = M(AreaObjectPageSel);
    if (!z)
        goto Chk1Row13;
    ++M(AreaObjectPageSel);
    ++M(AreaObjectPageLoc);

Chk1Row13:
    --y;
    a = M(W(AreaDataLow) + y);
    a &= 0x0f;
    compare(a, 0x0d);
    if (!z)
        goto Chk1Row14;
    ++y;
    a = M(W(AreaDataLow) + y);
    --y;
    a &= 0x40;
    if (!z)
        goto CheckRear;
    a = M(AreaObjectPageSel);
    if (!z)
        goto CheckRear;
    ++y;
    a = M(W(AreaDataLow) + y);
    a &= 0x1f;
    writeData(AreaObjectPageLoc, a);
    ++M(AreaObjectPageSel);
    goto NextAObj;

Chk1Row14:
    compare(a, 0x0e);
    if (!z)
        goto CheckRear;
    a = M(BackloadingFlag);
    if (!z)
        goto RdyDecode;

CheckRear:
    a = M(AreaObjectPageLoc);
    compare(a, M(CurrentPageLoc));
    if (!c)
        goto SetBehind;

RdyDecode:
    JSR(DecodeAreaData, 71);
    goto ChkLength;

SetBehind:
    ++M(BehindAreaParserFlag);

NextAObj:
    JSR(IncAreaObjOffset, 72);

ChkLength:
    x = M(BulletBill_FrenzyVar);
    a = M(AreaObjectLength + x);
    if (n)
        goto ProcLoopb;
    --M(AreaObjectLength + x);

ProcLoopb:
    --x;
    if (!n)
        goto ProcADLoop;
    a = M(BehindAreaParserFlag);
    if (!z)
        goto ProcessAreaData;
    a = M(BackloadingFlag);
    if (!z)
        goto ProcessAreaData;

EndAParse:
    goto Return;

IncAreaObjOffset:
    ++M(AreaDataOffset);
    ++M(AreaDataOffset);
    a = 0x00;
    writeData(AreaObjectPageSel, a);
    goto Return;

DecodeAreaData:
    a = M(AreaObjectLength + x);
    if (n)
        goto Chk1stB;
    y = M(AreaObjOffsetBuffer + x);

Chk1stB:
    x = 0x10;
    a = M(W(AreaDataLow) + y);
    compare(a, 0xfd);
    if (z)
        goto EndAParse;
    a &= 0x0f;
    compare(a, 0x0f);
    if (z)
        goto ChkRow14;
    x = 0x08;
    compare(a, 0x0c);
    if (z)
        goto ChkRow14;
    x = 0x00;

ChkRow14:
    writeData(World8, x);
    x = M(BulletBill_FrenzyVar);
    compare(a, 0x0e);
    if (!z)
        goto ChkRow13;
    a = 0x00;
    writeData(World8, a);
    a = 0x2e;
    if (!z)
        goto NormObj;

ChkRow13:
    compare(a, 0x0d);
    if (!z)
        goto ChkSRows;
    a = 0x22;
    writeData(World8, a);
    ++y;
    a = M(W(AreaDataLow) + y);
    a &= 0x40;
    if (z)
        goto LeavePar;
    a = M(W(AreaDataLow) + y);
    a &= 0x7f;
    compare(a, 0x4b);
    if (!z)
        goto Mask2MSB;
    ++M(LoopCommand);

Mask2MSB:
    a &= 0x3f;
    goto NormObj;

ChkSRows:
    compare(a, 0x0c);
    if (c)
        goto SpecObj;
    ++y;
    a = M(W(AreaDataLow) + y);
    a &= 0x70;
    if (!z)
        goto LrgObj;
    a = 0x16;
    writeData(World8, a);
    a = M(W(AreaDataLow) + y);
    a &= 0x0f;
    goto NormObj;

LrgObj:
    writeData(TitleScreenModeValue, a);
    compare(a, 0x70);
    if (!z)
        goto NotWPipe;
    a = M(W(AreaDataLow) + y);
    a &= 0x08;
    if (z)
        goto NotWPipe;
    a = 0x00;
    writeData(TitleScreenModeValue, a);

NotWPipe:
    a = M(TitleScreenModeValue);
    goto MoveAOId;

SpecObj:
    ++y;
    a = M(W(AreaDataLow) + y);
    a &= 0x70;

MoveAOId:
    a >>= 1;
    a >>= 1;
    a >>= 1;
    a >>= 1;

NormObj:
    writeData(TitleScreenModeValue, a);
    a = M(AreaObjectLength + x);
    if (!n)
        goto RunAObj;
    a = M(AreaObjectPageLoc);
    compare(a, M(CurrentPageLoc));
    if (z)
        goto InitRear;
    y = M(AreaDataOffset);
    a = M(W(AreaDataLow) + y);
    a &= 0x0f;
    compare(a, 0x0e);
    if (!z)
        goto LeavePar;
    a = M(BackloadingFlag);
    if (!z)
        goto StrAObj;

LeavePar:
    goto Return;

InitRear:
    a = M(BackloadingFlag);
    if (z)
        goto BackColC;
    a = 0x00;
    writeData(BackloadingFlag, a);
    writeData(BehindAreaParserFlag, a);
    writeData(BulletBill_FrenzyVar, a);

LoopCmdE:
    goto Return;

BackColC:
    y = M(AreaDataOffset);
    a = M(W(AreaDataLow) + y);
    a &= 0xf0;
    a >>= 1;
    a >>= 1;
    a >>= 1;
    a >>= 1;
    compare(a, M(CurrentColumnPos));
    if (!z)
        goto LeavePar;

StrAObj:
    a = M(AreaDataOffset);
    writeData(AreaObjOffsetBuffer + x, a);
    JSR(IncAreaObjOffset, 73);

RunAObj:
    a = M(TitleScreenModeValue);
    c = 0;
    a += M(World8);
    switch (a) {
        case 0: goto VerticalPipe;
        case 1: goto AreaStyleObject;
        case 2: goto RowOfBricks;
        case 3: goto RowOfSolidBlocks;
        case 4: goto RowOfCoins;
        case 5: goto ColumnOfBricks;
        case 6: goto ColumnOfSolidBlocks;
        case 7: goto VerticalPipe;
        case 8: goto Hole_Empty;
        case 9: goto PulleyRopeObject;
        case 10: goto Bridge_High;
        case 11: goto Bridge_Middle;
        case 12: goto Bridge_Low;
        case 13: goto Hole_Water;
        case 14: goto QuestionBlockRow_High;
        case 15: goto QuestionBlockRow_Low;
        case 16: goto EndlessRope;
        case 17: goto BalancePlatRope;
        case 18: goto CastleObject;
        case 19: goto StaircaseObject;
        case 20: goto ExitPipe;
        case 21: goto FlagBalls_Residual;
        case 22: goto QuestionBlock;
        case 23: goto QuestionBlock;
        case 24: goto QuestionBlock;
        case 25: goto Hidden1UpBlock;
        case 26: goto BrickWithItem;
        case 27: goto BrickWithItem;
        case 28: goto BrickWithItem;
        case 29: goto BrickWithCoins;
        case 30: goto BrickWithItem;
        case 31: goto WaterPipe;
        case 32: goto EmptyBlock;
        case 33: goto Jumpspring;
        case 34: goto IntroPipe;
        case 35: goto FlagpoleObject;
        case 36: goto AxeObj;
        case 37: goto ChainObj;
        case 38: goto CastleBridgeObj;
        case 39: goto ScrollLockObject_Warp;
        case 40: goto ScrollLockObject;
        case 41: goto ScrollLockObject;
        case 42: goto AreaFrenzy;
        case 43: goto AreaFrenzy;
        case 44: goto AreaFrenzy;
        case 45: goto LoopCmdE;
        case 46: goto AlterAreaAttributes;
    }

AlterAreaAttributes:
    y = M(AreaObjOffsetBuffer + x);
    ++y;
    a = M(W(AreaDataLow) + y);
    pha();
    a &= 0x40;
    if (!z)
        goto Alter2;
    pla();
    pha();
    a &= 0x0f;
    writeData(TerrainControl, a);
    pla();
    a &= 0x30;
    a >>= 1;
    a >>= 1;
    a >>= 1;
    a >>= 1;
    writeData(BackgroundScenery, a);
    goto Return;

Alter2:
    pla();
    a &= 0x07;
    compare(a, 0x04);
    if (!c)
        goto SetFore;
    writeData(BackgroundColorCtrl, a);
    a = 0x00;

SetFore:
    writeData(ForegroundScenery, a);
    goto Return;
    // ;--------------------------------

ScrollLockObject_Warp:
    x = 0x04;
    a = M(WorldNumber);
    if (z)
        goto WarpNum;
    ++x;
    y = M(AreaType);
    --y;
    if (!z)
        goto WarpNum;
    ++x;

WarpNum:
    a = x;
    writeData(WarpZoneControl, a);
    JSR(WriteGameText, 74);
    a = 0x0d;
    JSR(KillEnemies, 75);

ScrollLockObject:
    a = M(ScrollLock);
    a ^= 0x01;
    writeData(ScrollLock, a);
    goto Return;
    // ;--------------------------------
    // ;$00 - used to store enemy identifier in KillEnemies

KillEnemies:
    writeData(TitleScreenModeValue, a);
    a = 0x00;
    x = 0x04;

KillELoop:
    y = M(Fireworks + x);
    compare(y, M(TitleScreenModeValue));
    if (!z)
        goto NoKillE;
    writeData(RedParatroopa + x, a);

NoKillE:
    --x;
    if (!n)
        goto KillELoop;
    goto Return;
    // ;--------------------------------

AreaFrenzy:
    x = M(TitleScreenModeValue);
    a = M(0x9720 + x);
    y = 0x05;

FreCompLoop:
    --y;
    if (n)
        goto ExitAFrenzy;
    compare(a, M(Fireworks + y));
    if (!z)
        goto FreCompLoop;
    a = 0x00;

ExitAFrenzy:
    writeData(EnemyFrenzyQueue, a);
    goto Return;
    // ;--------------------------------
    // ;$06 - used by MushroomLedge to store length

AreaStyleObject:
    a = M(AreaStyle);
    switch (a) {
        case 0: goto TreeLedge;
        case 1: goto MushroomLedge;
        case 2: goto BulletBillCannon;
    }

TreeLedge:
    JSR(GetLrgObjAttrib, 76);
    a = M(AreaObjectLength + x);
    if (z)
        goto EndTreeL;
    if (!n)
        goto MidTreeL;
    a = y;
    writeData(AreaObjectLength + x, a);
    a = M(CurrentPageLoc);
    a |= M(CurrentColumnPos);
    if (z)
        goto MidTreeL;
    a = 0x16;
    goto NoUnder;

MidTreeL:
    x = M(World8);
    a = 0x17;
    writeData(MetatileBuffer + x, a);
    a = 0x4c;
    goto AllUnder;

EndTreeL:
    a = 0x18;
    goto NoUnder;

MushroomLedge:
    JSR(ChkLrgObjLength, 77);
    writeData(World7, y);
    if (!c)
        goto EndMushL;
    a = M(AreaObjectLength + x);
    a >>= 1;
    writeData(MushroomLedgeHalfLen + x, a);
    a = 0x19;
    goto NoUnder;

EndMushL:
    a = 0x1b;
    y = M(AreaObjectLength + x);
    if (z)
        goto NoUnder;
    a = M(MushroomLedgeHalfLen + x);
    writeData(World7, a);
    x = M(World8);
    a = 0x1a;
    writeData(MetatileBuffer + x, a);
    compare(y, M(World7));
    if (!z)
        goto MushLExit;
    ++x;
    a = 0x4f;
    writeData(MetatileBuffer + x, a);
    a = 0x50;

AllUnder:
    ++x;
    y = 0x0f;
    goto RenderUnderPart;

NoUnder:
    x = M(World8);
    y = 0x00;
    goto RenderUnderPart;
    // ;--------------------------------
    // ;tiles used by pulleys and rope object

PulleyRopeObject:
    JSR(ChkLrgObjLength, 78);
    y = 0x00;
    if (c)
        goto RenderPul;
    ++y;
    a = M(AreaObjectLength + x);
    if (!z)
        goto RenderPul;
    ++y;

RenderPul:
    a = M(PulleyRopeMetatiles + y);
    writeData(MetatileBuffer, a);

MushLExit:
    goto Return;
    // ;--------------------------------
    // ;$06 - used to store upper limit of rows for CastleObject

CastleObject:
    JSR(GetLrgObjAttrib, 79);
    writeData(World8, y);
    y = 0x04;
    JSR(ChkLrgObjFixedLength, 80);
    a = x;
    pha();
    y = M(AreaObjectLength + x);
    x = M(World8);
    a = 0x0b;
    writeData(World7, a);

CRendLoop:
    a = M(CastleMetatiles + y);
    writeData(MetatileBuffer + x, a);
    ++x;
    a = M(World7);
    if (z)
        goto ChkCFloor;
    ++y;
    ++y;
    ++y;
    ++y;
    ++y;
    --M(World7);

ChkCFloor:
    compare(x, 0x0b);
    if (!z)
        goto CRendLoop;
    pla();
    x = a;
    a = M(CurrentPageLoc);
    if (z)
        goto ExitCastle;
    a = M(AreaObjectLength + x);
    compare(a, 0x01);
    if (z)
        goto PlayerStop;
    y = M(World8);
    if (!z)
        goto NotTall;
    compare(a, 0x03);
    if (z)
        goto PlayerStop;

NotTall:
    compare(a, 0x02);
    if (!z)
        goto ExitCastle;
    JSR(GetAreaObjXPosition, 81);
    pha();
    JSR(FindEmptyEnemySlot, 82);
    pla();
    writeData(Enemy_X_Position + x, a);
    a = M(CurrentPageLoc);
    writeData(Enemy_PageLoc + x, a);
    a = 0x01;
    writeData(Enemy_Y_HighPos + x, a);
    writeData(RedParatroopa + x, a);
    a = 0x90;
    writeData(Enemy_Y_Position + x, a);
    a = 0x31;
    writeData(Fireworks + x, a);
    goto Return;

PlayerStop:
    y = 0x52;
    writeData(0x06ab, y);

ExitCastle:
    goto Return;
    // ;--------------------------------

WaterPipe:
    JSR(GetLrgObjAttrib, 83);
    y = M(AreaObjectLength + x);
    x = M(World8);
    a = 0x6b;
    writeData(MetatileBuffer + x, a);
    a = 0x6c;
    writeData(0x06a2 + x, a);
    goto Return;
    // ;--------------------------------
    // ;$05 - used to store length of vertical shaft in RenderSidewaysPipe
    // ;$06 - used to store leftover horizontal length in RenderSidewaysPipe
    // ; and vertical length in VerticalPipe and GetPipeHeight

IntroPipe:
    y = 0x03;
    JSR(ChkLrgObjFixedLength, 84);
    y = 0x0a;
    JSR(RenderSidewaysPipe, 85);
    if (c)
        goto NoBlankP;
    x = 0x06;

VPipeSectLoop:
    a = 0x00;
    writeData(MetatileBuffer + x, a);
    --x;
    if (!n)
        goto VPipeSectLoop;
    a = M(VerticalPipeData + y);
    writeData(0x06a8, a);

NoBlankP:
    goto Return;

ExitPipe:
    y = 0x03;
    JSR(ChkLrgObjFixedLength, 86);
    JSR(GetLrgObjAttrib, 87);

RenderSidewaysPipe:
    --y;
    --y;
    writeData(World6, y);
    y = M(AreaObjectLength + x);
    writeData(World7, y);
    x = M(World6);
    ++x;
    a = M(SidePipeShaftData + y);
    compare(a, 0x00);
    if (z)
        goto DrawSidePart;
    x = 0x00;
    y = M(World6);
    JSR(RenderUnderPart, 88);
    c = 0;

DrawSidePart:
    y = M(World7);
    a = M(SidePipeTopPart + y);
    writeData(MetatileBuffer + x, a);
    a = M(SidePipeBottomPart + y);
    writeData(0x06a2 + x, a);
    goto Return;

VerticalPipe:
    JSR(GetPipeHeight, 89);
    a = M(TitleScreenModeValue);
    if (z)
        goto WarpPipe;
    ++y;
    ++y;
    ++y;
    ++y;

WarpPipe:
    a = y;
    pha();
    a = M(AreaNumber);
    a |= M(WorldNumber);
    if (z)
        goto DrawPipe;
    y = M(AreaObjectLength + x);
    if (z)
        goto DrawPipe;
    JSR(FindEmptyEnemySlot, 90);
    if (c)
        goto DrawPipe;
    JSR(GetAreaObjXPosition, 91);
    c = 0;
    a += 0x08;
    writeData(Enemy_X_Position + x, a);
    a = M(CurrentPageLoc);
    a += 0x00;
    writeData(Enemy_PageLoc + x, a);
    a = 0x01;
    writeData(Enemy_Y_HighPos + x, a);
    writeData(RedParatroopa + x, a);
    JSR(GetAreaObjYPosition, 92);
    writeData(Enemy_Y_Position + x, a);
    a = 0x0d;
    writeData(Fireworks + x, a);
    JSR(InitPiranhaPlant, 93);

DrawPipe:
    pla();
    y = a;
    x = M(World8);
    a = M(VerticalPipeData + y);
    writeData(MetatileBuffer + x, a);
    ++x;
    a = M(data_98DF + y);
    y = M(World7);
    --y;
    goto RenderUnderPart;

GetPipeHeight:
    y = 0x01;
    JSR(ChkLrgObjFixedLength, 94);
    JSR(GetLrgObjAttrib, 95);
    a = y;
    a &= 0x07;
    writeData(World7, a);
    y = M(AreaObjectLength + x);
    goto Return;

FindEmptyEnemySlot:
    x = 0x00;

EmptyChkLoop:
    c = 0;
    a = M(RedParatroopa + x);
    if (z)
        goto ExitEmptyChk;
    ++x;
    compare(x, 0x05);
    if (!z)
        goto EmptyChkLoop;

ExitEmptyChk:
    goto Return;
    // ;--------------------------------

Hole_Water:
    JSR(ChkLrgObjLength, 96);
    a = 0x86;
    writeData(0x06ab, a);
    x = 0x0b;
    y = 0x01;
    a = 0x87;
    goto RenderUnderPart;
    // ;--------------------------------

QuestionBlockRow_High:
    a = 0x03;
    bit(M(0x07a9));
    goto loc_996D;

QuestionBlockRow_Low:
    a = 0x07;
loc_996D:
    pha();
    JSR(ChkLrgObjLength, 97);
    pla();
    x = a;
    a = 0xc0;
    writeData(MetatileBuffer + x, a);
    goto Return;
    // ;--------------------------------

Bridge_High:
    a = 0x06;
    bit(M(0x07a9));
    goto loc_997E;

Bridge_Middle:
    a = 0x07;
loc_997E:
    bit(M(0x09a9));
    goto loc_9981;

Bridge_Low:
    a = 0x09;
loc_9981:
    pha();
    JSR(ChkLrgObjLength, 98);
    pla();
    x = a;
    a = 0x0b;
    writeData(MetatileBuffer + x, a);
    ++x;
    y = 0x00;
    a = 0x63;
    goto RenderUnderPart;
    // ;--------------------------------

FlagBalls_Residual:
    JSR(GetLrgObjAttrib, 99);
    x = 0x02;
    a = 0x6d;
    goto RenderUnderPart;
    // ;--------------------------------

FlagpoleObject:
    a = 0x24;
    writeData(MetatileBuffer, a);
    x = 0x01;
    y = 0x08;
    a = 0x25;
    JSR(RenderUnderPart, 100);
    a = 0x61;
    writeData(0x06ab, a);
    JSR(GetAreaObjXPosition, 101);
    c = 1;
    a -= 0x08;
    writeData(0x008c, a);
    a = M(CurrentPageLoc);
    a -= 0x00;
    writeData(0x0073, a);
    a = 0x30;
    writeData(0x00d4, a);
    a = 0xb0;
    writeData(FlagpoleFNum_Y_Pos, a);
    a = 0x30;
    writeData(0x001b, a);
    ++M(FlyingCheepCheep);
    goto Return;
    // ;--------------------------------

EndlessRope:
    x = 0x00;
    y = 0x0f;
    goto DrawRope;

BalancePlatRope:
    a = x;
    pha();
    x = 0x01;
    y = 0x0f;
    a = 0x44;
    JSR(RenderUnderPart, 102);
    pla();
    x = a;
    JSR(GetLrgObjAttrib, 103);
    x = 0x01;

DrawRope:
    a = 0x40;
    goto RenderUnderPart;
    // ;--------------------------------

RowOfCoins:
    y = M(AreaType);
    a = M(CoinMetatileData + y);
    goto GetRow;
    // ;--------------------------------

CastleBridgeObj:
    y = 0x0c;
    JSR(ChkLrgObjFixedLength, 104);
    goto ChainObj;

AxeObj:
    a = 0x08;
    writeData(VRAM_Buffer_AddrCtrl, a);

ChainObj:
    y = M(TitleScreenModeValue);
    x = M(0x99f9 + y);
    a = M(0x99fc + y);
    goto ColObj;

EmptyBlock:
    JSR(GetLrgObjAttrib, 105);
    x = M(World8);
    a = 0xc4;

ColObj:
    y = 0x00;
    goto RenderUnderPart;
    // ;--------------------------------

RowOfBricks:
    y = M(AreaType);
    a = M(CloudTypeOverride);
    if (z)
        goto DrawBricks;
    y = 0x04;

DrawBricks:
    a = M(BrickMetatiles + y);
    goto GetRow;

RowOfSolidBlocks:
    y = M(AreaType);
    a = M(SolidBlockMetatiles + y);

GetRow:
    pha();
    JSR(ChkLrgObjLength, 106);

DrawRow:
    x = M(World8);
    y = 0x00;
    pla();
    goto RenderUnderPart;

ColumnOfBricks:
    y = M(AreaType);
    a = M(BrickMetatiles + y);
    goto GetRow2;

ColumnOfSolidBlocks:
    y = M(AreaType);
    a = M(SolidBlockMetatiles + y);

GetRow2:
    pha();
    JSR(GetLrgObjAttrib, 107);
    pla();
    x = M(World8);
    goto RenderUnderPart;
    // ;--------------------------------

BulletBillCannon:
    JSR(GetLrgObjAttrib, 108);
    x = M(World8);
    a = 0x64;
    writeData(MetatileBuffer + x, a);
    ++x;
    --y;
    if (n)
        goto SetupCannon;
    a = 0x65;
    writeData(MetatileBuffer + x, a);
    ++x;
    --y;
    if (n)
        goto SetupCannon;
    a = 0x66;
    JSR(RenderUnderPart, 109);

SetupCannon:
    x = M(Whirlpool_Offset);
    JSR(GetAreaObjYPosition, 110);
    writeData(Whirlpool_Length + x, a);
    a = M(CurrentPageLoc);
    writeData(Whirlpool_PageLoc + x, a);
    JSR(GetAreaObjXPosition, 111);
    writeData(Whirlpool_LeftExtent + x, a);
    ++x;
    compare(x, 0x06);
    if (!c)
        goto StrCOffset;
    x = 0x00;

StrCOffset:
    writeData(Whirlpool_Offset, x);
    goto Return;
    // ;--------------------------------

StaircaseObject:
    JSR(ChkLrgObjLength, 112);
    if (!c)
        goto NextStair;
    a = 0x09;
    writeData(StaircaseControl, a);

NextStair:
    --M(StaircaseControl);
    y = M(StaircaseControl);
    x = M(StaircaseRowData + y);
    a = M(StaircaseHeightData + y);
    y = a;
    a = 0x61;
    goto RenderUnderPart;
    // ;--------------------------------

Jumpspring:
    JSR(GetLrgObjAttrib, 113);
    JSR(FindEmptyEnemySlot, 114);
    JSR(GetAreaObjXPosition, 115);
    writeData(Enemy_X_Position + x, a);
    a = M(CurrentPageLoc);
    writeData(Enemy_PageLoc + x, a);
    JSR(GetAreaObjYPosition, 116);
    writeData(Enemy_Y_Position + x, a);
    writeData(ExplosionGfxCounter + x, a);
    a = 0x32;
    writeData(Fireworks + x, a);
    y = 0x01;
    writeData(Enemy_Y_HighPos + x, y);
    ++M(RedParatroopa + x);
    x = M(World8);
    a = 0x67;
    writeData(MetatileBuffer + x, a);
    a = 0x68;
    writeData(0x06a2 + x, a);
    goto Return;
    // ;--------------------------------
    // ;$07 - used to save ID of brick object

Hidden1UpBlock:
    a = M(Hidden1UpFlag);
    if (z)
        goto ExitDecBlock;
    a = 0x00;
    writeData(Hidden1UpFlag, a);
    goto BrickWithItem;

QuestionBlock:
    JSR(GetAreaObjectID, 117);
    goto DrawQBlk;

BrickWithCoins:
    a = 0x00;
    writeData(BrickCoinTimerFlag, a);

BrickWithItem:
    JSR(GetAreaObjectID, 118);
    writeData(World8, y);
    a = 0x00;
    y = M(AreaType);
    --y;
    if (z)
        goto BWithL;
    a = 0x05;

BWithL:
    c = 0;
    a += M(World8);
    y = a;

DrawQBlk:
    a = M(BrickQBlockMetatiles + y);
    pha();
    JSR(GetLrgObjAttrib, 119);
    goto DrawRow;

GetAreaObjectID:
    a = M(TitleScreenModeValue);
    c = 1;
    a -= 0x00;
    y = a;

ExitDecBlock:
    goto Return;
    // ;--------------------------------

Hole_Empty:
    JSR(ChkLrgObjLength, 120);
    if (!c)
        goto NoWhirlP;
    a = M(AreaType);
    if (!z)
        goto NoWhirlP;
    x = M(Whirlpool_Offset);
    JSR(GetAreaObjXPosition, 121);
    c = 1;
    a -= 0x10;
    writeData(Whirlpool_LeftExtent + x, a);
    a = M(CurrentPageLoc);
    a -= 0x00;
    writeData(Whirlpool_PageLoc + x, a);
    ++y;
    ++y;
    a = y;
    a <<= 1;
    a <<= 1;
    a <<= 1;
    a <<= 1;
    writeData(Whirlpool_Length + x, a);
    ++x;
    compare(x, 0x05);
    if (!c)
        goto StrWOffset;
    x = 0x00;

StrWOffset:
    writeData(Whirlpool_Offset, x);

NoWhirlP:
    x = M(AreaType);
    a = M(HoleMetatiles + x);
    x = 0x08;
    y = 0x0f;
    // ;--------------------------------

RenderUnderPart:
    writeData(AreaObjectHeight, y);
    y = M(MetatileBuffer + x);
    if (z)
        goto DrawThisRow;
    compare(y, 0x17);
    if (z)
        goto WaitOneRow;
    compare(y, 0x1a);
    if (z)
        goto WaitOneRow;
    compare(y, 0xc0);
    if (z)
        goto DrawThisRow;
    compare(y, 0xc0);
    if (c)
        goto WaitOneRow;
    compare(y, 0x54);
    if (!z)
        goto DrawThisRow;
    compare(a, 0x50);
    if (z)
        goto WaitOneRow;

DrawThisRow:
    writeData(MetatileBuffer + x, a);

WaitOneRow:
    ++x;
    compare(x, 0x0d);
    if (c)
        goto ExitUPartR;
    y = M(AreaObjectHeight);
    --y;
    if (!n)
        goto RenderUnderPart;

ExitUPartR:
    goto Return;
    // ;--------------------------------

ChkLrgObjLength:
    JSR(GetLrgObjAttrib, 122);

ChkLrgObjFixedLength:
    a = M(AreaObjectLength + x);
    c = 0;
    if (!n)
        goto LenSet;
    a = y;
    writeData(AreaObjectLength + x, a);
    c = 1;

LenSet:
    goto Return;

GetLrgObjAttrib:
    y = M(AreaObjOffsetBuffer + x);
    a = M(W(AreaDataLow) + y);
    a &= 0x0f;
    writeData(World8, a);
    ++y;
    a = M(W(AreaDataLow) + y);
    a &= 0x0f;
    y = a;
    goto Return;
    // ;--------------------------------

GetAreaObjXPosition:
    a = M(CurrentColumnPos);
    a <<= 1;
    a <<= 1;
    a <<= 1;
    a <<= 1;
    goto Return;
    // ;--------------------------------

GetAreaObjYPosition:
    a = M(World8);
    a <<= 1;
    a <<= 1;
    a <<= 1;
    a <<= 1;
    c = 0;
    a += 0x20;
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$06-$07 - used to store block buffer address used as indirect

GetBlockBufferAddr:
    pha();
    a >>= 1;
    a >>= 1;
    a >>= 1;
    a >>= 1;
    y = a;
    a = M(data_9BDF + y);
    writeData(World8, a);
    pla();
    a &= 0x0f;
    c = 0;
    a += M(BlockBufferAddr + y);
    writeData(World7, a);
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;unused space
    // ;-------------------------------------------------------------------------------------
    // ;-------------------------------------------------------------------------------------

LoadAreaPointer:
    JSR(FindAreaPointer, 123);
    writeData(AreaPointer, a);

GetAreaType:
    a &= 0x60;
    a <<= 1;
    a.rol();
    a.rol();
    a.rol();
    writeData(AreaType, a);
    goto Return;

FindAreaPointer:
    y = M(WorldNumber);
    a = M(WorldAddrOffsets + y);
    c = 0;
    a += M(AreaNumber);
    y = a;
    a = M(AreaAddrOffsets + y);
    goto Return;

GetAreaDataAddrs:
    a = M(AreaPointer);
    JSR(GetAreaType, 124);
    y = a;
    a = M(AreaPointer);
    a &= 0x1f;
    writeData(AreaAddrsLOffset, a);
    a = M(EnemyAddrHOffsets + y);
    c = 0;
    a += M(AreaAddrsLOffset);
    y = a;
    a = M(EnemyDataAddrLow + y);
    writeData(EnemyDataLow, a);
    a = M(EnemyDataAddrHigh + y);
    writeData(EnemyDataHigh, a);
    y = M(AreaType);
    a = M(AreaDataHOffsets + y);
    c = 0;
    a += M(AreaAddrsLOffset);
    y = a;
    a = M(AreaDataAddrLow + y);
    writeData(AreaDataLow, a);
    a = M(AreaDataAddrHigh + y);
    writeData(AreaDataHigh, a);
    y = 0x00;
    a = M(W(AreaDataLow) + y);
    pha();
    a &= 0x07;
    compare(a, 0x04);
    if (!c)
        goto StoreFore;
    writeData(BackgroundColorCtrl, a);
    a = 0x00;

StoreFore:
    writeData(ForegroundScenery, a);
    pla();
    pha();
    a &= 0x38;
    a >>= 1;
    a >>= 1;
    a >>= 1;
    writeData(PlayerEntranceCtrl, a);
    pla();
    a &= 0xc0;
    c = 0;
    a.rol();
    a.rol();
    a.rol();
    writeData(GameTimerSetting, a);
    ++y;
    a = M(W(AreaDataLow) + y);
    pha();
    a &= 0x0f;
    writeData(TerrainControl, a);
    pla();
    pha();
    a &= 0x30;
    a >>= 1;
    a >>= 1;
    a >>= 1;
    a >>= 1;
    writeData(BackgroundScenery, a);
    pla();
    a &= 0xc0;
    c = 0;
    a.rol();
    a.rol();
    a.rol();
    compare(a, 0x03);
    if (!z)
        goto StoreStyle;
    writeData(CloudTypeOverride, a);
    a = 0x00;

StoreStyle:
    writeData(AreaStyle, a);
    a = M(AreaDataLow);
    c = 0;
    a += 0x02;
    writeData(AreaDataLow, a);
    a = M(AreaDataHigh);
    a += 0x00;
    writeData(AreaDataHigh, a);
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;GAME LEVELS DATA
    // ;bonus area data offsets, included here for comparison purposes
    // ;underground bonus area  - c2
    // ;cloud area 1 (day)      - 2b
    // ;cloud area 2 (night)    - 34
    // ;water area (5-2/6-2)    - 00
    // ;water area (8-4)        - 02
    // ;warp zone area (4-2)    - 2f
    // ;ENEMY OBJECT DATA
    // ;level 1-4/6-4
    // ;level 4-4
    // ;level 2-4/5-4
    // ;level 3-4
    // ;level 7-4
    // ;level 8-4
    // ;level 3-3
    // ;level 8-3
    // ;level 4-1
    // ;level 6-2
    // ;level 3-1
    // ;level 1-1
    // ;level 1-3/5-3
    // ;level 2-3/7-3
    // ;level 2-1
    // ;end of data terminator here is also used by pipe intro area
    // ;level 5-1
    // ;cloud level used in levels 2-1 and 5-2
    // ;level 4-3
    // ;level 6-3
    // ;level 6-1
    // ;warp zone area used in level 4-2
    // ;level 8-1
    // ;level 5-2
    // ;level 8-2
    // ;level 7-1
    // ;cloud level used in levels 3-1 and 6-2
    // ;level 3-2
    // ;level 1-2
    // ;level 4-2
    // ;underground bonus rooms area used in many levels
    // ;water area used in levels 5-2 and 6-2
    // ;level 2-2/7-2
    // ;water area used in level 8-4
    // ;AREA OBJECT DATA
    // ;level 1-4/6-4
    // ;level 4-4
    // ;level 2-4/5-4
    // ;level 3-4
    // ;level 7-4
    // ;level 8-4
    // ;level 3-3
    // ;level 8-3
    // ;level 4-1
    // ;level 6-2
    // ;level 3-1
    // ;level 1-1
    // ;level 1-3/5-3
    // ;level 2-3/7-3
    // ;level 2-1
    // ;pipe intro area
    // ;level 5-1
    // ;cloud level used in levels 2-1 and 5-2
    // ;level 4-3
    // ;level 6-3
    // ;level 6-1
    // ;warp zone area used in level 4-2
    // ;level 8-1
    // ;level 5-2
    // ;level 8-2
    // ;level 7-1
    // ;cloud level used in levels 3-1 and 6-2
    // ;level 3-2
    // ;level 1-2
    // ;level 4-2
    // ;underground bonus rooms area used in many levels
    // ;water area used in levels 5-2 and 6-2
    // ;level 2-2/7-2
    // ;water area used in level 8-4
    // ;-------------------------------------------------------------------------------------
    // ;unused space
    // ;-------------------------------------------------------------------------------------
    // ;indirect jump routine called when
    // ;$0770 is set to 1

GameMode:
    a = M(OperMode_Task);
    switch (a) {
        case 0: goto InitializeArea;
        case 1: goto ScreenRoutines;
        case 2: goto SecondaryGameSetup;
        case 3: goto GameCoreRoutine;
    }

GameCoreRoutine:
    x = M(CurrentPlayer);
    a = M(SavedJoypad1Bits + x);
    writeData(SavedJoypad1Bits, a);
    JSR(GameRoutines, 125);
    a = M(OperMode_Task);
    compare(a, 0x03);
    if (c)
        goto GameEngine;
    goto Return;

GameEngine:
    JSR(ProcFireball_Bubble, 126);
    x = 0x00;

ProcELoop:
    writeData(BulletBill_FrenzyVar, x);
    JSR(EnemiesAndLoopsCore, 127);
    JSR(FloateyNumbersRoutine, 128);
    ++x;
    compare(x, 0x06);
    if (!z)
        goto ProcELoop;
    JSR(GetPlayerOffscreenBits, 129);
    JSR(RelativePlayerPosition, 130);
    JSR(PlayerGfxHandler, 131);
    JSR(BlockObjMT_Updater, 132);
    x = 0x01;
    writeData(BulletBill_FrenzyVar, x);
    JSR(BlockObjectsCore, 133);
    --x;
    writeData(BulletBill_FrenzyVar, x);
    JSR(BlockObjectsCore, 134);
    JSR(MiscObjectsCore, 135);
    JSR(ProcessCannons, 136);
    JSR(ProcessWhirlpools, 137);
    JSR(FlagpoleRoutine, 138);
    JSR(RunGameTimer, 139);
    JSR(ColorRotation, 140);
    a = M(Player_Y_HighPos);
    compare(a, 0x02);
    if (!n)
        goto NoChgMus;
    a = M(StarInvincibleTimer);
    if (z)
        goto ClrPlrPal;
    compare(a, 0x04);
    if (!z)
        goto NoChgMus;
    a = M(IntervalTimerControl);
    if (!z)
        goto NoChgMus;
    JSR(GetAreaMusic, 141);

NoChgMus:
    y = M(StarInvincibleTimer);
    a = M(TallEnemy);
    compare(y, 0x08);
    if (c)
        goto CycleTwo;
    a >>= 1;
    a >>= 1;

CycleTwo:
    a >>= 1;
    JSR(CyclePlayerPalette, 142);
    goto SaveAB;

ClrPlrPal:
    JSR(ResetPalStar, 143);

SaveAB:
    a = M(GreyCheepCheep);
    writeData(PiranhaPlant, a);
    a = 0x00;
    writeData(Podoboo, a);

UpdScrollVar:
    a = M(VRAM_Buffer_AddrCtrl);
    compare(a, 0x06);
    if (z)
        goto ExitEng;
    a = M(AreaParserTaskNum);
    if (!z)
        goto RunParser;
    a = M(ScrollThirtyTwo);
    compare(a, 0x20);
    if (n)
        goto ExitEng;
    a = M(ScrollThirtyTwo);
    a -= 0x20;
    writeData(ScrollThirtyTwo, a);
    a = 0x00;
    writeData(VRAM_Buffer2_Offset, a);

RunParser:
    JSR(AreaParserTaskHandler, 144);

ExitEng:
    goto Return;
    // ;-------------------------------------------------------------------------------------

ScrollHandler:
    a = M(Player_X_Scroll);
    c = 0;
    a += M(Platform_X_Scroll);
    writeData(Player_X_Scroll, a);
    a = M(ScrollLock);
    if (!z)
        goto InitScrlAmt;
    a = M(Player_Pos_ForScroll);
    compare(a, 0x50);
    if (!c)
        goto InitScrlAmt;
    a = M(SideCollisionTimer);
    if (!z)
        goto InitScrlAmt;
    y = M(Player_X_Scroll);
    --y;
    if (n)
        goto InitScrlAmt;
    ++y;
    compare(y, 0x02);
    if (!c)
        goto ChkNearMid;
    --y;

ChkNearMid:
    a = M(Player_Pos_ForScroll);
    compare(a, 0x70);
    if (!c)
        goto ScrollScreen;
    y = M(Player_X_Scroll);

ScrollScreen:
    a = y;
    writeData(ScrollAmount, a);
    c = 0;
    a += M(ScrollThirtyTwo);
    writeData(ScrollThirtyTwo, a);
    a = y;
    c = 0;
    a += M(ScreenLeft_X_Pos);
    writeData(ScreenLeft_X_Pos, a);
    writeData(HorizontalScroll, a);
    a = M(ScreenLeft_PageLoc);
    a += 0x00;
    writeData(ScreenLeft_PageLoc, a);
    a &= 0x01;
    writeData(TitleScreenModeValue, a);
    a = M(Mirror_PPU_CTRL_REG1);
    a &= 0xfe;
    a |= M(TitleScreenModeValue);
    writeData(Mirror_PPU_CTRL_REG1, a);
    JSR(GetScreenPosition, 145);
    a = 0x08;
    writeData(ScrollIntervalTimer, a);
    goto ChkPOffscr;

InitScrlAmt:
    a = 0x00;
    writeData(ScrollAmount, a);

ChkPOffscr:
    x = 0x00;
    JSR(GetXOffscreenBits, 146);
    writeData(TitleScreenModeValue, a);
    y = 0x00;
    a <<= 1;
    if (c)
        goto KeepOnscr;
    ++y;
    a = M(TitleScreenModeValue);
    a &= 0x20;
    if (z)
        goto InitPlatScrl;

KeepOnscr:
    a = M(ScreenLeft_X_Pos + y);
    c = 1;
    a -= M(X_SubtracterData + y);
    writeData(Player_X_Position, a);
    a = M(ScreenLeft_PageLoc + y);
    a -= 0x00;
    writeData(Player_PageLoc, a);
    a = M(Podoboo);
    compare(a, M(OffscrJoypadBitsData + y));
    if (z)
        goto InitPlatScrl;
    a = 0x00;
    writeData(Player_X_Speed, a);

InitPlatScrl:
    a = 0x00;
    writeData(Platform_X_Scroll, a);
    goto Return;
    // ;-------------------------------------------------------------------------------------

GetScreenPosition:
    a = M(ScreenLeft_X_Pos);
    c = 0;
    a += 0xff;
    writeData(ScreenRight_X_Pos, a);
    a = M(ScreenLeft_PageLoc);
    a += 0x00;
    writeData(ScreenRight_PageLoc, a);
    goto Return;
    // ;-------------------------------------------------------------------------------------

GameRoutines:
    a = M(GreenParatroopaJump);
    switch (a) {
        case 0: goto Entrance_GameTimerSetup;
        case 1: goto Vine_AutoClimb;
        case 2: goto SideExitPipeEntry;
        case 3: goto VerticalPipeEntry;
        case 4: goto FlagpoleSlide;
        case 5: goto PlayerEndLevel;
        case 6: goto PlayerLoseLife;
        case 7: goto PlayerEntrance;
        case 8: goto PlayerCtrlRoutine;
        case 9: goto PlayerChangeSize;
        case 10: goto PlayerInjuryBlink;
        case 11: goto PlayerDeath;
        case 12: goto PlayerFireFlower;
    }

PlayerEntrance:
    a = M(AltEntranceControl);
    compare(a, 0x02);
    if (z)
        goto EntrMode2;
    a = 0x00;
    y = M(Player_Y_Position);
    compare(y, 0x30);
    if (!c)
        goto AutoControlPlayer;
    a = M(PlayerEntranceCtrl);
    compare(a, 0x06);
    if (z)
        goto ChkBehPipe;
    compare(a, 0x07);
    if (!z)
        goto PlayerRdy;

ChkBehPipe:
    a = M(Player_SprAttrib);
    if (!z)
        goto IntroEntr;
    a = 0x01;
    goto AutoControlPlayer;

IntroEntr:
    JSR(EnterSidePipe, 147);
    --M(ChangeAreaTimer);
    if (!z)
        goto ExitEntr;
    ++M(DisableIntermediate);
    goto NextArea;

EntrMode2:
    a = M(JoypadOverride);
    if (!z)
        goto VineEntr;
    a = 0xff;
    JSR(MovePlayerYAxis, 148);
    a = M(Player_Y_Position);
    compare(a, 0x91);
    if (!c)
        goto PlayerRdy;
    goto Return;

VineEntr:
    a = M(VineHeight);
    compare(a, 0x60);
    if (!z)
        goto ExitEntr;
    a = M(Player_Y_Position);
    compare(a, 0x99);
    y = 0x00;
    a = 0x01;
    if (!c)
        goto OffVine;
    a = 0x03;
    writeData(Player_State, a);
    ++y;
    a = 0x08;
    writeData(0x05b4, a);

OffVine:
    writeData(DisableCollisionDet, y);
    JSR(AutoControlPlayer, 149);
    a = M(Player_X_Position);
    compare(a, 0x48);
    if (!c)
        goto ExitEntr;

PlayerRdy:
    a = 0x08;
    writeData(GreenParatroopaJump, a);
    a = 0x01;
    writeData(BulletBill_CannonVar, a);
    a >>= 1;
    writeData(AltEntranceControl, a);
    writeData(DisableCollisionDet, a);
    writeData(JoypadOverride, a);

ExitEntr:
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$07 - used to hold upper limit of high byte when player falls down hole

AutoControlPlayer:
    writeData(SavedJoypad1Bits, a);

PlayerCtrlRoutine:
    a = M(GreenParatroopaJump);
    compare(a, 0x0b);
    if (z)
        goto SizeChk;
    a = M(AreaType);
    if (!z)
        goto SaveJoyp;
    y = M(Player_Y_HighPos);
    --y;
    if (!z)
        goto DisJoyp;
    a = M(Player_Y_Position);
    compare(a, 0xd0);
    if (!c)
        goto SaveJoyp;

DisJoyp:
    a = 0x00;
    writeData(SavedJoypad1Bits, a);

SaveJoyp:
    a = M(SavedJoypad1Bits);
    a &= 0xc0;
    writeData(GreyCheepCheep, a);
    a = M(SavedJoypad1Bits);
    a &= 0x03;
    writeData(Podoboo, a);
    a = M(SavedJoypad1Bits);
    a &= 0x0c;
    writeData(RedCheepCheep, a);
    a &= 0x04;
    if (z)
        goto SizeChk;
    a = M(Player_State);
    if (!z)
        goto SizeChk;
    y = M(Podoboo);
    if (z)
        goto SizeChk;
    a = 0x00;
    writeData(Podoboo, a);
    writeData(RedCheepCheep, a);

SizeChk:
    JSR(PlayerMovementSubs, 150);
    y = 0x01;
    a = M(PlayerSize);
    if (!z)
        goto ChkMoveDir;
    y = 0x00;
    a = M(CrouchingFlag);
    if (z)
        goto ChkMoveDir;
    y = 0x02;

ChkMoveDir:
    writeData(Player_BoundBoxCtrl, y);
    a = 0x01;
    y = M(Player_X_Speed);
    if (z)
        goto PlayerSubs;
    if (!n)
        goto SetMoveDir;
    a <<= 1;

SetMoveDir:
    writeData(Player_MovingDir, a);

PlayerSubs:
    JSR(ScrollHandler, 151);
    JSR(GetPlayerOffscreenBits, 152);
    JSR(RelativePlayerPosition, 153);
    x = 0x00;
    JSR(BoundingBoxCore, 154);
    JSR(PlayerBGCollision, 155);
    a = M(Player_Y_Position);
    compare(a, 0x40);
    if (!c)
        goto PlayerHole;
    a = M(GreenParatroopaJump);
    compare(a, 0x05);
    if (z)
        goto PlayerHole;
    compare(a, 0x07);
    if (z)
        goto PlayerHole;
    compare(a, 0x04);
    if (!c)
        goto PlayerHole;
    a = M(Player_SprAttrib);
    a &= 0xdf;
    writeData(Player_SprAttrib, a);

PlayerHole:
    a = M(Player_Y_HighPos);
    compare(a, 0x02);
    if (n)
        goto ExitCtrl;
    x = 0x01;
    writeData(ScrollLock, x);
    y = 0x04;
    writeData(World8, y);
    x = 0x00;
    y = M(GameTimerExpiredFlag);
    if (!z)
        goto HoleDie;
    y = M(CloudTypeOverride);
    if (!z)
        goto ChkHoleX;

HoleDie:
    ++x;
    y = M(GreenParatroopaJump);
    compare(y, 0x0b);
    if (z)
        goto ChkHoleX;
    y = M(DeathMusicLoaded);
    if (!z)
        goto HoleBottom;
    ++y;
    writeData(EventMusicQueue, y);
    writeData(DeathMusicLoaded, y);

HoleBottom:
    y = 0x06;
    writeData(World8, y);

ChkHoleX:
    compare(a, M(World8));
    if (n)
        goto ExitCtrl;
    --x;
    if (n)
        goto CloudExit;
    y = M(EventMusicBuffer);
    if (!z)
        goto ExitCtrl;
    a = 0x06;
    writeData(GreenParatroopaJump, a);

ExitCtrl:
    goto Return;

CloudExit:
    a = 0x00;
    writeData(JoypadOverride, a);
    JSR(SetEntr, 156);
    ++M(AltEntranceControl);
    goto Return;
    // ;-------------------------------------------------------------------------------------

Vine_AutoClimb:
    a = M(Player_Y_HighPos);
    if (!z)
        goto AutoClimb;
    a = M(Player_Y_Position);
    compare(a, 0xe4);
    if (!c)
        goto SetEntr;

AutoClimb:
    a = 0x08;
    writeData(JoypadOverride, a);
    y = 0x03;
    writeData(Player_State, y);
    goto AutoControlPlayer;

SetEntr:
    a = 0x02;
    writeData(AltEntranceControl, a);
    goto ChgAreaMode;
    // ;-------------------------------------------------------------------------------------

VerticalPipeEntry:
    a = 0x01;
    JSR(MovePlayerYAxis, 157);
    JSR(ScrollHandler, 158);
    y = 0x00;
    a = M(WarpZoneControl);
    if (!z)
        goto ChgAreaPipe;
    ++y;
    a = M(AreaType);
    compare(a, 0x03);
    if (!z)
        goto ChgAreaPipe;
    ++y;
    goto ChgAreaPipe;

MovePlayerYAxis:
    c = 0;
    a += M(Player_Y_Position);
    writeData(Player_Y_Position, a);
    goto Return;
    // ;-------------------------------------------------------------------------------------

SideExitPipeEntry:
    JSR(EnterSidePipe, 159);
    y = 0x02;

ChgAreaPipe:
    --M(ChangeAreaTimer);
    if (!z)
        goto ExitCAPipe;
    writeData(AltEntranceControl, y);

ChgAreaMode:
    ++M(DisableScreenFlag);
    a = 0x00;
    writeData(OperMode_Task, a);
    writeData(Sprite0HitDetectFlag, a);

ExitCAPipe:
    goto Return;

EnterSidePipe:
    a = 0x08;
    writeData(Player_X_Speed, a);
    y = 0x01;
    a = M(Player_X_Position);
    a &= 0x0f;
    if (!z)
        goto RightPipe;
    writeData(Player_X_Speed, a);
    y = a;

RightPipe:
    a = y;
    JSR(AutoControlPlayer, 160);
    goto Return;
    // ;-------------------------------------------------------------------------------------

PlayerChangeSize:
    a = M(TimerControl);
    compare(a, 0xf8);
    if (!z)
        goto EndChgSize;
    goto InitChangeSize;

EndChgSize:
    compare(a, 0xc4);
    if (!z)
        goto ExitChgSize;
    JSR(DonePlayerTask, 161);

ExitChgSize:
    goto Return;
    // ;-------------------------------------------------------------------------------------

PlayerInjuryBlink:
    a = M(TimerControl);
    compare(a, 0xf0);
    if (c)
        goto ExitBlink;
    compare(a, 0xc8);
    if (z)
        goto DonePlayerTask;
    goto PlayerCtrlRoutine;

ExitBlink:
    if (!z)
        goto ExitBoth;

InitChangeSize:
    y = M(PlayerChangeSizeFlag);
    if (!z)
        goto ExitBoth;
    writeData(PlayerAnimCtrl, y);
    ++M(PlayerChangeSizeFlag);
    a = M(PlayerSize);
    a ^= 0x01;
    writeData(PlayerSize, a);

ExitBoth:
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$00 - used in CyclePlayerPalette to store current palette to cycle

PlayerDeath:
    a = M(TimerControl);
    compare(a, 0xf0);
    if (c)
        goto ExitDeath;
    goto PlayerCtrlRoutine;

DonePlayerTask:
    a = 0x00;
    writeData(TimerControl, a);
    a = 0x08;
    writeData(GreenParatroopaJump, a);
    goto Return;

PlayerFireFlower:
    a = M(TimerControl);
    compare(a, 0xc0);
    if (z)
        goto ResetPalFireFlower;
    a = M(TallEnemy);
    a >>= 1;
    a >>= 1;

CyclePlayerPalette:
    a &= 0x03;
    writeData(TitleScreenModeValue, a);
    a = M(Player_SprAttrib);
    a &= 0xfc;
    a |= M(TitleScreenModeValue);
    writeData(Player_SprAttrib, a);
    goto Return;

ResetPalFireFlower:
    JSR(DonePlayerTask, 162);

ResetPalStar:
    a = M(Player_SprAttrib);
    a &= 0xfc;
    writeData(Player_SprAttrib, a);
    goto Return;

ExitDeath:
    goto Return;
    // ;-------------------------------------------------------------------------------------

FlagpoleSlide:
    a = M(0x001b);
    compare(a, 0x30);
    if (!z)
        goto NoFPObj;
    a = M(FlagpoleSoundQueue);
    writeData(Square1SoundQueue, a);
    a = 0x00;
    writeData(FlagpoleSoundQueue, a);
    y = M(Player_Y_Position);
    compare(y, 0x9e);
    if (c)
        goto SlidePlayer;
    a = 0x04;

SlidePlayer:
    goto AutoControlPlayer;

NoFPObj:
    ++M(GreenParatroopaJump);
    goto Return;
    // ;-------------------------------------------------------------------------------------

PlayerEndLevel:
    a = 0x01;
    JSR(AutoControlPlayer, 163);
    a = M(Player_Y_Position);
    compare(a, 0xae);
    if (!c)
        goto ChkStop;
    a = M(ScrollLock);
    if (z)
        goto ChkStop;
    a = 0x20;
    writeData(EventMusicQueue, a);
    a = 0x00;
    writeData(ScrollLock, a);

ChkStop:
    a = M(Player_CollisionBits);
    a >>= 1;
    if (c)
        goto RdyNextA;
    a = M(StarFlagTaskControl);
    if (!z)
        goto InCastle;
    ++M(StarFlagTaskControl);

InCastle:
    a = 0x20;
    writeData(Player_SprAttrib, a);

RdyNextA:
    a = M(StarFlagTaskControl);
    compare(a, 0x05);
    if (!z)
        goto ExitNA;
    ++M(LevelNumber);
    a = M(LevelNumber);
    compare(a, 0x03);
    if (!z)
        goto NextArea;
    y = M(WorldNumber);
    a = M(CoinTallyFor1Ups);
    compare(a, M(Hidden1UpCoinAmts + y));
    if (!c)
        goto NextArea;
    ++M(Hidden1UpFlag);

NextArea:
    ++M(AreaNumber);
    JSR(LoadAreaPointer, 164);
    ++M(FetchNewGameTimerFlag);
    JSR(ChgAreaMode, 165);
    writeData(HalfwayPage, a);
    a = 0x80;
    writeData(EventMusicQueue, a);

ExitNA:
    goto Return;
    // ;-------------------------------------------------------------------------------------

PlayerMovementSubs:
    a = 0x00;
    y = M(PlayerSize);
    if (!z)
        goto SetCrouch;
    a = M(Player_State);
    if (!z)
        goto ProcMove;
    a = M(RedCheepCheep);
    a &= 0x04;

SetCrouch:
    writeData(CrouchingFlag, a);

ProcMove:
    JSR(PlayerPhysicsSub, 166);
    a = M(PlayerChangeSizeFlag);
    if (!z)
        goto NoMoveSub;
    a = M(Player_State);
    compare(a, 0x03);
    if (z)
        goto loc_B34E;
    y = 0x18;
    writeData(ClimbSideTimer, y);
loc_B34E:
    switch (a) {
        case 0: goto OnGroundStateSub;
        case 1: goto JumpSwimSub;
        case 2: goto FallingSub;
        case 3: goto ClimbingSub;
    }

NoMoveSub:
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$00 - used by ClimbingSub to store high vertical adder

OnGroundStateSub:
    JSR(GetPlayerAnimSpeed, 167);
    a = M(Podoboo);
    if (z)
        goto GndMove;
    writeData(BulletBill_CannonVar, a);

GndMove:
    JSR(ImposeFriction, 168);
    JSR(MovePlayerHorizontally, 169);
    writeData(Player_X_Scroll, a);
    goto Return;
    // ;--------------------------------

FallingSub:
    a = M(VerticalForceDown);
    writeData(VerticalForce, a);
    goto LRAir;
    // ;--------------------------------

JumpSwimSub:
    y = M(Player_Y_Speed);
    if (!n)
        goto DumpFall;
    a = M(GreyCheepCheep);
    a &= 0x80;
    a &= M(PiranhaPlant);
    if (!z)
        goto ProcSwim;
    a = M(JumpOrigin_Y_Position);
    c = 1;
    a -= M(Player_Y_Position);
    compare(a, M(DiffToHaltJump));
    if (!c)
        goto ProcSwim;

DumpFall:
    a = M(VerticalForceDown);
    writeData(VerticalForce, a);

ProcSwim:
    a = M(SwimmingFlag);
    if (z)
        goto LRAir;
    JSR(GetPlayerAnimSpeed, 170);
    a = M(Player_Y_Position);
    compare(a, 0x14);
    if (c)
        goto LRWater;
    a = 0x18;
    writeData(VerticalForce, a);

LRWater:
    a = M(Podoboo);
    if (z)
        goto LRAir;
    writeData(BulletBill_CannonVar, a);

LRAir:
    a = M(Podoboo);
    if (z)
        goto JSMove;
    JSR(ImposeFriction, 171);

JSMove:
    JSR(MovePlayerHorizontally, 172);
    writeData(Player_X_Scroll, a);
    a = M(GreenParatroopaJump);
    compare(a, 0x0b);
    if (!z)
        goto ExitMov1;
    a = 0x28;
    writeData(VerticalForce, a);

ExitMov1:
    goto MovePlayerVertically;
    // ;--------------------------------

ClimbingSub:
    a = M(Player_YMF_Dummy);
    c = 0;
    a += M(Player_Y_MoveForce);
    writeData(Player_YMF_Dummy, a);
    y = 0x00;
    a = M(Player_Y_Speed);
    if (!n)
        goto MoveOnVine;
    --y;

MoveOnVine:
    writeData(TitleScreenModeValue, y);
    a += M(Player_Y_Position);
    writeData(Player_Y_Position, a);
    a = M(Player_Y_HighPos);
    a += M(TitleScreenModeValue);
    writeData(Player_Y_HighPos, a);
    a = M(Podoboo);
    a &= M(Player_CollisionBits);
    if (z)
        goto InitCSTimer;
    y = M(ClimbSideTimer);
    if (!z)
        goto ExitCSub;
    y = 0x18;
    writeData(ClimbSideTimer, y);
    x = 0x00;
    y = M(BulletBill_CannonVar);
    a >>= 1;
    if (c)
        goto ClimbFD;
    ++x;
    ++x;

ClimbFD:
    --y;
    if (z)
        goto CSetFDir;
    ++x;

CSetFDir:
    a = M(Player_X_Position);
    c = 0;
    a += M(ClimbAdderLow + x);
    writeData(Player_X_Position, a);
    a = M(Player_PageLoc);
    a += M(ClimbAdderHigh + x);
    writeData(Player_PageLoc, a);
    a = M(Podoboo);
    a ^= 0x03;
    writeData(BulletBill_CannonVar, a);

ExitCSub:
    goto Return;

InitCSTimer:
    writeData(ClimbSideTimer, a);
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$00 - used to store offset to friction data

PlayerPhysicsSub:
    a = M(Player_State);
    compare(a, 0x03);
    if (!z)
        goto CheckForJumping;
    y = 0x00;
    a = M(RedCheepCheep);
    a &= M(Player_CollisionBits);
    if (z)
        goto ProcClimb;
    ++y;
    a &= 0x08;
    if (!z)
        goto ProcClimb;
    ++y;

ProcClimb:
    x = M(Climb_Y_MForceData + y);
    writeData(Player_Y_MoveForce, x);
    a = 0x08;
    x = M(Climb_Y_SpeedData + y);
    writeData(Player_Y_Speed, x);
    if (n)
        goto SetCAnim;
    a >>= 1;

SetCAnim:
    writeData(PlayerAnimTimerSet, a);
    goto Return;

CheckForJumping:
    a = M(JumpspringAnimCtrl);
    if (!z)
        goto NoJump;
    a = M(GreyCheepCheep);
    a &= 0x80;
    if (z)
        goto NoJump;
    a &= M(PiranhaPlant);
    if (z)
        goto ProcJumping;

NoJump:
    goto X_Physics;

ProcJumping:
    a = M(Player_State);
    if (z)
        goto InitJS;
    a = M(SwimmingFlag);
    if (z)
        goto NoJump;
    a = M(JumpSwimTimer);
    if (!z)
        goto InitJS;
    a = M(Player_Y_Speed);
    if (!n)
        goto InitJS;
    goto X_Physics;

InitJS:
    a = 0x20;
    writeData(JumpSwimTimer, a);
    y = 0x00;
    writeData(Player_YMF_Dummy, y);
    writeData(Player_Y_MoveForce, y);
    a = M(Player_Y_HighPos);
    writeData(JumpOrigin_Y_HighPos, a);
    a = M(Player_Y_Position);
    writeData(JumpOrigin_Y_Position, a);
    a = 0x01;
    writeData(Player_State, a);
    a = M(Player_XSpeedAbsolute);
    compare(a, 0x09);
    if (!c)
        goto ChkWtr;
    ++y;
    compare(a, 0x10);
    if (!c)
        goto ChkWtr;
    ++y;
    compare(a, 0x19);
    if (!c)
        goto ChkWtr;
    ++y;
    compare(a, 0x1c);
    if (!c)
        goto ChkWtr;
    ++y;

ChkWtr:
    a = 0x01;
    writeData(DiffToHaltJump, a);
    a = M(SwimmingFlag);
    if (z)
        goto GetYPhy;
    y = 0x05;
    a = M(Whirlpool_Flag);
    if (z)
        goto GetYPhy;
    ++y;

GetYPhy:
    a = M(JumpMForceData + y);
    writeData(VerticalForce, a);
    a = M(FallMForceData + y);
    writeData(VerticalForceDown, a);
    a = M(InitMForceData + y);
    writeData(Player_Y_MoveForce, a);
    a = M(PlayerYSpdData + y);
    writeData(Player_Y_Speed, a);
    a = M(SwimmingFlag);
    if (z)
        goto PJumpSnd;
    a = 0x04;
    writeData(Square1SoundQueue, a);
    a = M(Player_Y_Position);
    compare(a, 0x14);
    if (c)
        goto X_Physics;
    a = 0x00;
    writeData(Player_Y_Speed, a);
    goto X_Physics;

PJumpSnd:
    a = 0x01;
    y = M(PlayerSize);
    if (z)
        goto SJumpSnd;
    a = 0x80;

SJumpSnd:
    writeData(Square1SoundQueue, a);

X_Physics:
    y = 0x00;
    writeData(TitleScreenModeValue, y);
    a = M(Player_State);
    if (z)
        goto ProcPRun;
    a = M(Player_XSpeedAbsolute);
    compare(a, 0x19);
    if (c)
        goto GetXPhy;
    if (!c)
        goto ChkRFast;

ProcPRun:
    ++y;
    a = M(AreaType);
    if (z)
        goto ChkRFast;
    --y;
    a = M(Podoboo);
    compare(a, M(Player_MovingDir));
    if (!z)
        goto ChkRFast;
    a = M(GreyCheepCheep);
    a &= 0x40;
    if (!z)
        goto SetRTmr;
    a = M(RunningTimer);
    if (!z)
        goto GetXPhy;

ChkRFast:
    ++y;
    ++M(TitleScreenModeValue);
    a = M(RunningSpeed);
    if (!z)
        goto FastXSp;
    a = M(Player_XSpeedAbsolute);
    compare(a, 0x21);
    if (!c)
        goto GetXPhy;

FastXSp:
    ++M(TitleScreenModeValue);
    goto GetXPhy;

SetRTmr:
    a = 0x0a;
    writeData(RunningTimer, a);

GetXPhy:
    a = M(MaxLeftXSpdData + y);
    writeData(MaximumLeftSpeed, a);
    a = M(GreenParatroopaJump);
    compare(a, 0x07);
    if (!z)
        goto GetXPhy2;
    y = 0x03;

GetXPhy2:
    a = M(MaxRightXSpdData + y);
    writeData(MaximumRightSpeed, a);
    y = M(TitleScreenModeValue);
    a = M(FrictionData + y);
    writeData(FrictionAdderLow, a);
    a = 0x00;
    writeData(FrictionAdderHigh, a);
    a = M(BulletBill_CannonVar);
    compare(a, M(Player_MovingDir));
    if (z)
        goto ExitPhy;
    M(FrictionAdderLow) <<= 1;
    M(FrictionAdderHigh).rol();

ExitPhy:
    goto Return;
    // ;-------------------------------------------------------------------------------------

GetPlayerAnimSpeed:
    y = 0x00;
    a = M(Player_XSpeedAbsolute);
    compare(a, 0x1c);
    if (c)
        goto SetRunSpd;
    ++y;
    compare(a, 0x0e);
    if (c)
        goto ChkSkid;
    ++y;

ChkSkid:
    a = M(SavedJoypad1Bits);
    a &= 0x7f;
    if (z)
        goto SetAnimSpd;
    a &= 0x03;
    compare(a, M(Player_MovingDir));
    if (!z)
        goto ProcSkid;
    a = 0x00;

SetRunSpd:
    writeData(RunningSpeed, a);
    goto SetAnimSpd;

ProcSkid:
    a = M(Player_XSpeedAbsolute);
    compare(a, 0x0b);
    if (c)
        goto SetAnimSpd;
    a = M(BulletBill_CannonVar);
    writeData(Player_MovingDir, a);
    a = 0x00;
    writeData(Player_X_Speed, a);
    writeData(Player_X_MoveForce, a);

SetAnimSpd:
    a = M(PlayerAnimTmrData + y);
    writeData(PlayerAnimTimerSet, a);
    goto Return;
    // ;-------------------------------------------------------------------------------------

ImposeFriction:
    a &= M(Player_CollisionBits);
    compare(a, 0x00);
    if (!z)
        goto JoypFrict;
    a = M(Player_X_Speed);
    if (z)
        goto SetAbsSpd;
    if (!n)
        goto RghtFrict;
    if (n)
        goto LeftFrict;

JoypFrict:
    a >>= 1;
    if (!c)
        goto RghtFrict;

LeftFrict:
    a = M(Player_X_MoveForce);
    c = 0;
    a += M(FrictionAdderLow);
    writeData(Player_X_MoveForce, a);
    a = M(Player_X_Speed);
    a += M(FrictionAdderHigh);
    writeData(Player_X_Speed, a);
    compare(a, M(MaximumRightSpeed));
    if (n)
        goto XSpdSign;
    a = M(MaximumRightSpeed);
    writeData(Player_X_Speed, a);
    goto SetAbsSpd;

RghtFrict:
    a = M(Player_X_MoveForce);
    c = 1;
    a -= M(FrictionAdderLow);
    writeData(Player_X_MoveForce, a);
    a = M(Player_X_Speed);
    a -= M(FrictionAdderHigh);
    writeData(Player_X_Speed, a);
    compare(a, M(MaximumLeftSpeed));
    if (!n)
        goto XSpdSign;
    a = M(MaximumLeftSpeed);
    writeData(Player_X_Speed, a);

XSpdSign:
    compare(a, 0x00);
    if (!n)
        goto SetAbsSpd;
    a ^= 0xff;
    c = 0;
    a += 0x01;

SetAbsSpd:
    writeData(Player_XSpeedAbsolute, a);
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$00 - used to store downward movement force in FireballObjCore
    // ;$02 - used to store maximum vertical speed in FireballObjCore
    // ;$07 - used to store pseudorandom bit in BubbleCheck

ProcFireball_Bubble:
    a = M(PlayerStatus);
    compare(a, 0x02);
    if (!c)
        goto ProcAirBubbles;
    a = M(GreyCheepCheep);
    a &= 0x40;
    if (z)
        goto ProcFireballs;
    a &= M(PiranhaPlant);
    if (!z)
        goto ProcFireballs;
    a = M(FireballCounter);
    a &= 0x01;
    x = a;
    a = M(Fireball_State + x);
    if (!z)
        goto ProcFireballs;
    y = M(Player_Y_HighPos);
    --y;
    if (!z)
        goto ProcFireballs;
    a = M(CrouchingFlag);
    if (!z)
        goto ProcFireballs;
    a = M(Player_State);
    compare(a, 0x03);
    if (z)
        goto ProcFireballs;
    a = 0x20;
    writeData(Square1SoundQueue, a);
    a = 0x02;
    writeData(Fireball_State + x, a);
    y = M(PlayerAnimTimerSet);
    writeData(FireballThrowingTimer, y);
    --y;
    writeData(PlayerAnimTimer, y);
    ++M(FireballCounter);

ProcFireballs:
    x = 0x00;
    JSR(FireballObjCore, 173);
    x = 0x01;
    JSR(FireballObjCore, 174);

ProcAirBubbles:
    a = M(AreaType);
    if (!z)
        goto BublExit;
    x = 0x02;

BublLoop:
    writeData(BulletBill_FrenzyVar, x);
    JSR(BubbleCheck, 175);
    JSR(RelativeBubblePosition, 176);
    JSR(GetBubbleOffscreenBits, 177);
    JSR(DrawBubble, 178);
    --x;
    if (!n)
        goto BublLoop;

BublExit:
    goto Return;

FireballObjCore:
    writeData(BulletBill_FrenzyVar, x);
    a = M(Fireball_State + x);
    a <<= 1;
    if (c)
        goto FireballExplosion;
    y = M(Fireball_State + x);
    if (z)
        goto NoFBall;
    --y;
    if (z)
        goto RunFB;
    a = M(Player_X_Position);
    a += 0x04;
    writeData(Fireball_X_Position + x, a);
    a = M(Player_PageLoc);
    a += 0x00;
    writeData(Fireball_PageLoc + x, a);
    a = M(Player_Y_Position);
    writeData(Fireball_Y_Position + x, a);
    a = 0x01;
    writeData(Fireball_Y_HighPos + x, a);
    y = M(BulletBill_CannonVar);
    --y;
    a = M(FireballXSpdData + y);
    writeData(Fireball_X_Speed + x, a);
    a = 0x04;
    writeData(Fireball_Y_Speed + x, a);
    a = 0x07;
    writeData(Fireball_BoundBoxCtrl + x, a);
    --M(Fireball_State + x);

RunFB:
    a = x;
    c = 0;
    a += 0x07;
    x = a;
    a = 0x50;
    writeData(TitleScreenModeValue, a);
    a = 0x03;
    writeData(VictoryModeValue, a);
    a = 0x00;
    JSR(ImposeGravity, 179);
    JSR(MoveObjectHorizontally, 180);
    x = M(BulletBill_FrenzyVar);
    JSR(RelativeFireballPosition, 181);
    JSR(GetFireballOffscreenBits, 182);
    JSR(GetFireballBoundBox, 183);
    JSR(FireballBGCollision, 184);
    a = M(FBall_OffscreenBits);
    a &= 0xcc;
    if (!z)
        goto EraseFB;
    JSR(FireballEnemyCollision, 185);
    goto DrawFireball;

EraseFB:
    a = 0x00;
    writeData(Fireball_State + x, a);

NoFBall:
    goto Return;

FireballExplosion:
    JSR(RelativeFireballPosition, 186);
    goto DrawExplosion_Fireball;

BubbleCheck:
    a = M(0x07a8 + x);
    a &= 0x01;
    writeData(World8, a);
    a = M(Bubble_Y_Position + x);
    compare(a, 0xf8);
    if (!z)
        goto MoveBubl;
    a = M(AirBubbleTimer);
    if (!z)
        goto ExitBubl;

SetupBubble:
    y = 0x00;
    a = M(BulletBill_CannonVar);
    a >>= 1;
    if (!c)
        goto PosBubl;
    y = 0x08;

PosBubl:
    a = y;
    a += M(Player_X_Position);
    writeData(Bubble_X_Position + x, a);
    a = M(Player_PageLoc);
    a += 0x00;
    writeData(Bubble_PageLoc + x, a);
    a = M(Player_Y_Position);
    c = 0;
    a += 0x08;
    writeData(Bubble_Y_Position + x, a);
    a = 0x01;
    writeData(Bubble_Y_HighPos + x, a);
    y = M(World8);
    a = M(BubbleTimerData + y);
    writeData(AirBubbleTimer, a);

MoveBubl:
    y = M(World8);
    a = M(Bubble_YMF_Dummy + x);
    c = 1;
    a -= M(Bubble_MForceData + y);
    writeData(Bubble_YMF_Dummy + x, a);
    a = M(Bubble_Y_Position + x);
    a -= 0x00;
    compare(a, 0x20);
    if (c)
        goto Y_Bubl;
    a = 0xf8;

Y_Bubl:
    writeData(Bubble_Y_Position + x, a);

ExitBubl:
    goto Return;
    // ;-------------------------------------------------------------------------------------

RunGameTimer:
    a = M(OperMode);
    if (z)
        goto ExGTimer;
    a = M(GreenParatroopaJump);
    compare(a, 0x08);
    if (!c)
        goto ExGTimer;
    compare(a, 0x0b);
    if (z)
        goto ExGTimer;
    a = M(Player_Y_HighPos);
    compare(a, 0x02);
    if (c)
        goto ExGTimer;
    a = M(GameTimerCtrlTimer);
    if (!z)
        goto ExGTimer;
    a = M(GameTimerDisplay);
    a |= M(0x07f9);
    a |= M(0x07fa);
    if (z)
        goto TimeUpOn;
    y = M(GameTimerDisplay);
    --y;
    if (!z)
        goto ResGTCtrl;
    a = M(0x07f9);
    a |= M(0x07fa);
    if (!z)
        goto ResGTCtrl;
    a = 0x40;
    writeData(EventMusicQueue, a);

ResGTCtrl:
    a = 0x18;
    writeData(GameTimerCtrlTimer, a);
    y = 0x23;
    a = 0xff;
    writeData(0x0139, a);
    JSR(DigitsMathRoutine, 187);
    a = 0xa4;
    goto PrintStatusBarNumbers;

TimeUpOn:
    writeData(PlayerStatus, a);
    JSR(ForceInjury, 188);
    ++M(GameTimerExpiredFlag);

ExGTimer:
    goto Return;
    // ;-------------------------------------------------------------------------------------

WarpZoneObject:
    a = M(ScrollLock);
    if (z)
        goto ExGTimer;
    a = M(Player_Y_Position);
    a &= M(Player_Y_HighPos);
    if (!z)
        goto ExGTimer;
    writeData(ScrollLock, a);
    ++M(WarpZoneControl);
    goto EraseEnemyObject;
    // ;-------------------------------------------------------------------------------------
    // ;$00 - used in WhirlpoolActivate to store whirlpool length / 2, page location of center of whirlpool
    // ;and also to store movement force exerted on player
    // ;$01 - used in ProcessWhirlpools to store page location of right extent of whirlpool
    // ;and in WhirlpoolActivate to store center of whirlpool
    // ;$02 - used in ProcessWhirlpools to store right extent of whirlpool and in
    // ;WhirlpoolActivate to store maximum vertical speed

ProcessWhirlpools:
    a = M(AreaType);
    if (!z)
        goto ExitWh;
    writeData(Whirlpool_Flag, a);
    a = M(TimerControl);
    if (!z)
        goto ExitWh;
    y = 0x04;

WhLoop:
    a = M(Whirlpool_LeftExtent + y);
    c = 0;
    a += M(Whirlpool_Length + y);
    writeData(VictoryModeValue, a);
    a = M(Whirlpool_PageLoc + y);
    if (z)
        goto NextWh;
    a += 0x00;
    writeData(GameModeValue, a);
    a = M(Player_X_Position);
    c = 1;
    a -= M(Whirlpool_LeftExtent + y);
    a = M(Player_PageLoc);
    a -= M(Whirlpool_PageLoc + y);
    if (n)
        goto NextWh;
    a = M(VictoryModeValue);
    c = 1;
    a -= M(Player_X_Position);
    a = M(GameModeValue);
    a -= M(Player_PageLoc);
    if (!n)
        goto WhirlpoolActivate;

NextWh:
    --y;
    if (!n)
        goto WhLoop;

ExitWh:
    goto Return;

WhirlpoolActivate:
    a = M(Whirlpool_Length + y);
    a >>= 1;
    writeData(TitleScreenModeValue, a);
    a = M(Whirlpool_LeftExtent + y);
    c = 0;
    a += M(TitleScreenModeValue);
    writeData(GameModeValue, a);
    a = M(Whirlpool_PageLoc + y);
    a += 0x00;
    writeData(TitleScreenModeValue, a);
    a = M(TallEnemy);
    a >>= 1;
    if (!c)
        goto WhPull;
    a = M(GameModeValue);
    c = 1;
    a -= M(Player_X_Position);
    a = M(TitleScreenModeValue);
    a -= M(Player_PageLoc);
    if (!n)
        goto LeftWh;
    a = M(Player_X_Position);
    c = 1;
    a -= 0x01;
    writeData(Player_X_Position, a);
    a = M(Player_PageLoc);
    a -= 0x00;
    goto SetPWh;

LeftWh:
    a = M(Player_CollisionBits);
    a >>= 1;
    if (!c)
        goto WhPull;
    a = M(Player_X_Position);
    c = 0;
    a += 0x01;
    writeData(Player_X_Position, a);
    a = M(Player_PageLoc);
    a += 0x00;

SetPWh:
    writeData(Player_PageLoc, a);

WhPull:
    a = 0x10;
    writeData(TitleScreenModeValue, a);
    a = 0x01;
    writeData(Whirlpool_Flag, a);
    writeData(VictoryModeValue, a);
    a >>= 1;
    x = a;
    goto ImposeGravity;
    // ;-------------------------------------------------------------------------------------

FlagpoleRoutine:
    x = 0x05;
    writeData(BulletBill_FrenzyVar, x);
    a = M(Fireworks + x);
    compare(a, 0x30);
    if (!z)
        goto ExitFlagP;
    a = M(GreenParatroopaJump);
    compare(a, 0x04);
    if (!z)
        goto SkipScore;
    a = M(Player_State);
    compare(a, 0x03);
    if (!z)
        goto SkipScore;
    a = M(Enemy_Y_Position + x);
    compare(a, 0xaa);
    if (c)
        goto GiveFPScr;
    a = M(Player_Y_Position);
    compare(a, 0xa2);
    if (c)
        goto GiveFPScr;
    a = M(PiranhaPlantUpYPos + x);
    a += 0xff;
    writeData(PiranhaPlantUpYPos + x, a);
    a = M(Enemy_Y_Position + x);
    a += 0x01;
    writeData(Enemy_Y_Position + x, a);
    a = M(FlagpoleFNum_YMFDummy);
    c = 1;
    a -= 0xff;
    writeData(FlagpoleFNum_YMFDummy, a);
    a = M(FlagpoleFNum_Y_Pos);
    a -= 0x01;
    writeData(FlagpoleFNum_Y_Pos, a);

SkipScore:
    goto FPGfx;

GiveFPScr:
    y = M(FlagpoleScore);
    a = M(FlagpoleScoreMods + y);
    x = M(FlagpoleScoreDigits + y);
    writeData(DigitModifier + x, a);
    JSR(AddToScore, 189);
    a = 0x05;
    writeData(GreenParatroopaJump, a);

FPGfx:
    JSR(GetEnemyOffscreenBits, 190);
    JSR(RelativeEnemyPosition, 191);
    JSR(FlagpoleGfxHandler, 192);

ExitFlagP:
    goto Return;
    // ;-------------------------------------------------------------------------------------

JumpspringHandler:
    JSR(GetEnemyOffscreenBits, 193);
    a = M(TimerControl);
    if (!z)
        goto DrawJSpr;
    a = M(JumpspringAnimCtrl);
    if (z)
        goto DrawJSpr;
    y = a;
    --y;
    a = y;
    a &= 0x02;
    if (!z)
        goto DownJSpr;
    ++M(Player_Y_Position);
    ++M(Player_Y_Position);
    goto PosJSpr;

DownJSpr:
    --M(Player_Y_Position);
    --M(Player_Y_Position);

PosJSpr:
    a = M(ExplosionGfxCounter + x);
    c = 0;
    a += M(Jumpspring_Y_PosData + y);
    writeData(Enemy_Y_Position + x, a);
    compare(y, 0x01);
    if (!c)
        goto BounceJS;
    a = M(GreyCheepCheep);
    a &= 0x80;
    if (z)
        goto BounceJS;
    a &= M(PiranhaPlant);
    if (!z)
        goto BounceJS;
    a = 0xf4;
    writeData(JumpspringForce, a);

BounceJS:
    compare(y, 0x03);
    if (!z)
        goto DrawJSpr;
    a = M(JumpspringForce);
    writeData(Player_Y_Speed, a);
    a = 0x00;
    writeData(JumpspringAnimCtrl, a);

DrawJSpr:
    JSR(RelativeEnemyPosition, 194);
    JSR(EnemyGfxHandler, 195);
    JSR(OffscreenBoundsCheck, 196);
    a = M(JumpspringAnimCtrl);
    if (z)
        goto ExJSpring;
    a = M(JumpspringTimer);
    if (!z)
        goto ExJSpring;
    a = 0x04;
    writeData(JumpspringTimer, a);
    ++M(JumpspringAnimCtrl);

ExJSpring:
    goto Return;
    // ;-------------------------------------------------------------------------------------

Setup_Vine:
    a = 0x2f;
    writeData(Fireworks + x, a);
    a = 0x01;
    writeData(RedParatroopa + x, a);
    a = M(Block_PageLoc + y);
    writeData(Enemy_PageLoc + x, a);
    a = M(Block_X_Position + y);
    writeData(Enemy_X_Position + x, a);
    a = M(Block_Y_Position + y);
    writeData(Enemy_Y_Position + x, a);
    y = M(VineFlagOffset);
    if (!z)
        goto NextVO;
    writeData(VineStart_Y_Position, a);

NextVO:
    a = x;
    writeData(VineObjOffset + y, a);
    ++M(VineFlagOffset);
    a = 0x04;
    writeData(Square2SoundQueue, a);
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$06-$07 - used as address to block buffer data
    // ;$02 - used as vertical high nybble of block buffer offset

VineObjectHandler:
    compare(x, 0x05);
    if (!z)
        goto ExitVH;
    y = M(VineFlagOffset);
    --y;
    a = M(VineHeight);
    compare(a, M(VineHeightData + y));
    if (z)
        goto RunVSubs;
    a = M(TallEnemy);
    a >>= 1;
    a >>= 1;
    if (!c)
        goto RunVSubs;
    a = M(0x00d4);
    a -= 0x01;
    writeData(0x00d4, a);
    ++M(VineHeight);

RunVSubs:
    a = M(VineHeight);
    compare(a, 0x08);
    if (!c)
        goto ExitVH;
    JSR(RelativeEnemyPosition, 197);
    JSR(GetEnemyOffscreenBits, 198);
    y = 0x00;

VDrawLoop:
    JSR(DrawVine, 199);
    ++y;
    compare(y, M(VineFlagOffset));
    if (!z)
        goto VDrawLoop;
    a = M(Enemy_OffscreenBits);
    a &= 0x0c;
    if (z)
        goto WrCMTile;
    --y;

KillVine:
    x = M(VineObjOffset + y);
    JSR(EraseEnemyObject, 200);
    --y;
    if (!n)
        goto KillVine;
    writeData(VineFlagOffset, a);
    writeData(VineHeight, a);

WrCMTile:
    a = M(VineHeight);
    compare(a, 0x20);
    if (!c)
        goto ExitVH;
    x = 0x06;
    a = 0x01;
    y = 0x1b;
    JSR(BlockBufferCollision, 201);
    y = M(VictoryModeValue);
    compare(y, 0xd0);
    if (c)
        goto ExitVH;
    a = M(W(World7) + y);
    if (!z)
        goto ExitVH;
    a = 0x26;
    writeData(W(World7) + y, a);

ExitVH:
    x = M(BulletBill_FrenzyVar);
    goto Return;
    // ;-------------------------------------------------------------------------------------

ProcessCannons:
    a = M(AreaType);
    if (z)
        goto ExCannon;
    x = 0x02;

ThreeSChk:
    writeData(BulletBill_FrenzyVar, x);
    a = M(RedParatroopa + x);
    if (!z)
        goto Chk_BB;
    a = M(0x07a8 + x);
    y = M(SecondaryHardMode);
    a &= M(CannonBitmasks + y);
    compare(a, 0x06);
    if (c)
        goto Chk_BB;
    y = a;
    a = M(Whirlpool_PageLoc + y);
    if (z)
        goto Chk_BB;
    a = M(Whirlpool_Flag + y);
    if (z)
        goto FireCannon;
    a -= 0x00;
    writeData(Whirlpool_Flag + y, a);
    goto Chk_BB;

FireCannon:
    a = M(TimerControl);
    if (!z)
        goto Chk_BB;
    a = 0x0e;
    writeData(Whirlpool_Flag + y, a);
    a = M(Whirlpool_PageLoc + y);
    writeData(Enemy_PageLoc + x, a);
    a = M(Whirlpool_LeftExtent + y);
    writeData(Enemy_X_Position + x, a);
    a = M(Whirlpool_Length + y);
    c = 1;
    a -= 0x08;
    writeData(Enemy_Y_Position + x, a);
    a = 0x01;
    writeData(Enemy_Y_HighPos + x, a);
    writeData(RedParatroopa + x, a);
    a >>= 1;
    writeData(Enemy_State + x, a);
    a = 0x09;
    writeData(Enemy_BoundBoxCtrl + x, a);
    a = 0x33;
    writeData(Fireworks + x, a);
    goto Next3Slt;

Chk_BB:
    a = M(Fireworks + x);
    compare(a, 0x33);
    if (!z)
        goto Next3Slt;
    JSR(OffscreenBoundsCheck, 202);
    a = M(RedParatroopa + x);
    if (z)
        goto Next3Slt;
    JSR(GetEnemyOffscreenBits, 203);
    JSR(BulletBillHandler, 204);

Next3Slt:
    --x;
    if (!n)
        goto ThreeSChk;

ExCannon:
    goto Return;
    // ;--------------------------------

BulletBillHandler:
    a = M(TimerControl);
    if (!z)
        goto RunBBSubs;
    a = M(Enemy_State + x);
    if (!z)
        goto ChkDSte;
    a = M(Enemy_OffscreenBits);
    a &= 0x0c;
    compare(a, 0x0c);
    if (z)
        goto KillBB;
    y = 0x01;
    JSR(PlayerEnemyDiff, 205);
    if (n)
        goto SetupBB;
    ++y;

SetupBB:
    writeData(Enemy_MovingDir + x, y);
    --y;
    a = M(BulletBillXSpdData + y);
    writeData(ExplosionGfxCounter + x, a);
    a = M(TitleScreenModeValue);
    a += 0x28;
    compare(a, 0x50);
    if (!c)
        goto KillBB;
    a = 0x01;
    writeData(Enemy_State + x, a);
    a = 0x0a;
    writeData(EnemyFrameTimer + x, a);
    a = 0x08;
    writeData(Square2SoundQueue, a);

ChkDSte:
    a = M(Enemy_State + x);
    a &= 0x20;
    if (z)
        goto BBFly;
    JSR(MoveD_EnemyVertically, 206);

BBFly:
    JSR(MoveEnemyHorizontally, 207);

RunBBSubs:
    JSR(GetEnemyOffscreenBits, 208);
    JSR(RelativeEnemyPosition, 209);
    JSR(GetEnemyBoundBox, 210);
    JSR(PlayerEnemyCollision, 211);
    goto EnemyGfxHandler;

KillBB:
    JSR(EraseEnemyObject, 212);
    goto Return;
    // ;-------------------------------------------------------------------------------------

SpawnHammerObj:
    a = M(0x07a8);
    a &= 0x07;
    if (!z)
        goto SetMOfs;
    a = M(0x07a8);
    a &= 0x08;

SetMOfs:
    y = a;
    a = M(Misc_State + y);
    if (!z)
        goto NoHammer;
    x = M(HammerEnemyOfsData + y);
    a = M(RedParatroopa + x);
    if (!z)
        goto NoHammer;
    x = M(BulletBill_FrenzyVar);
    a = x;
    writeData(HammerEnemyOffset + y, a);
    a = 0x90;
    writeData(Misc_State + y, a);
    a = 0x07;
    writeData(Misc_BoundBoxCtrl + y, a);
    c = 1;
    goto Return;

NoHammer:
    x = M(BulletBill_FrenzyVar);
    c = 0;
    goto Return;
    // ;--------------------------------
    // ;$00 - used to set downward force
    // ;$01 - used to set upward force (residual)
    // ;$02 - used to set maximum speed

ProcHammerObj:
    a = M(TimerControl);
    if (!z)
        goto RunHSubs;
    a = M(Misc_State + x);
    a &= 0x7f;
    y = M(HammerEnemyOffset + x);
    compare(a, 0x02);
    if (z)
        goto SetHSpd;
    if (c)
        goto SetHPos;
    a = x;
    c = 0;
    a += 0x0d;
    x = a;
    a = 0x10;
    writeData(TitleScreenModeValue, a);
    a = 0x0f;
    writeData(GameModeValue, a);
    a = 0x04;
    writeData(VictoryModeValue, a);
    a = 0x00;
    JSR(ImposeGravity, 213);
    JSR(MoveObjectHorizontally, 214);
    x = M(BulletBill_FrenzyVar);
    goto RunAllH;

SetHSpd:
    a = 0xfe;
    writeData(Misc_Y_Speed + x, a);
    a = M(Enemy_State + y);
    a &= 0xf7;
    writeData(Enemy_State + y, a);
    x = M(Enemy_MovingDir + y);
    --x;
    a = M(HammerXSpdData + x);
    x = M(BulletBill_FrenzyVar);
    writeData(Misc_X_Speed + x, a);

SetHPos:
    --M(Misc_State + x);
    a = M(Enemy_X_Position + y);
    c = 0;
    a += 0x02;
    writeData(Misc_X_Position + x, a);
    a = M(Enemy_PageLoc + y);
    a += 0x00;
    writeData(Misc_PageLoc + x, a);
    a = M(Enemy_Y_Position + y);
    c = 1;
    a -= 0x0a;
    writeData(Misc_Y_Position + x, a);
    a = 0x01;
    writeData(Misc_Y_HighPos + x, a);
    if (!z)
        goto RunHSubs;

RunAllH:
    JSR(PlayerHammerCollision, 215);

RunHSubs:
    JSR(GetMiscOffscreenBits, 216);
    JSR(RelativeMiscPosition, 217);
    JSR(GetMiscBoundBox, 218);
    JSR(DrawHammer, 219);
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$02 - used to store vertical high nybble offset from block buffer routine
    // ;$06 - used to store low byte of block buffer address

CoinBlock:
    JSR(FindEmptyMiscSlot, 220);
    a = M(Block_PageLoc + x);
    writeData(Misc_PageLoc + y, a);
    a = M(Block_X_Position + x);
    a |= 0x05;
    writeData(Misc_X_Position + y, a);
    a = M(Block_Y_Position + x);
    a -= 0x10;
    writeData(Misc_Y_Position + y, a);
    goto JCoinC;

SetupJumpCoin:
    JSR(FindEmptyMiscSlot, 221);
    a = M(Block_PageLoc2 + x);
    writeData(Misc_PageLoc + y, a);
    a = M(World7);
    a <<= 1;
    a <<= 1;
    a <<= 1;
    a <<= 1;
    a |= 0x05;
    writeData(Misc_X_Position + y, a);
    a = M(VictoryModeValue);
    a += 0x20;
    writeData(Misc_Y_Position + y, a);

JCoinC:
    a = 0xfb;
    writeData(Misc_Y_Speed + y, a);
    a = 0x01;
    writeData(Misc_Y_HighPos + y, a);
    writeData(Misc_State + y, a);
    writeData(Square2SoundQueue, a);
    writeData(BulletBill_FrenzyVar, x);
    JSR(GiveOneCoin, 222);
    ++M(CoinTallyFor1Ups);
    goto Return;

FindEmptyMiscSlot:
    y = 0x08;

FMiscLoop:
    a = M(Misc_State + y);
    if (z)
        goto UseMiscS;
    --y;
    compare(y, 0x05);
    if (!z)
        goto FMiscLoop;
    y = 0x08;

UseMiscS:
    writeData(JumpCoinMiscOffset, y);
    goto Return;
    // ;-------------------------------------------------------------------------------------

MiscObjectsCore:
    x = 0x08;

MiscLoop:
    writeData(BulletBill_FrenzyVar, x);
    a = M(Misc_State + x);
    if (z)
        goto MiscLoopBack;
    a <<= 1;
    if (!c)
        goto ProcJumpCoin;
    JSR(ProcHammerObj, 223);
    goto MiscLoopBack;
    // ;--------------------------------
    // ;$00 - used to set downward force
    // ;$01 - used to set upward force (residual)
    // ;$02 - used to set maximum speed

ProcJumpCoin:
    y = M(Misc_State + x);
    --y;
    if (z)
        goto JCoinRun;
    ++M(Misc_State + x);
    a = M(Misc_X_Position + x);
    c = 0;
    a += M(ScrollAmount);
    writeData(Misc_X_Position + x, a);
    a = M(Misc_PageLoc + x);
    a += 0x00;
    writeData(Misc_PageLoc + x, a);
    a = M(Misc_State + x);
    compare(a, 0x30);
    if (!z)
        goto RunJCSubs;
    a = 0x00;
    writeData(Misc_State + x, a);
    goto MiscLoopBack;

JCoinRun:
    a = x;
    c = 0;
    a += 0x0d;
    x = a;
    a = 0x50;
    writeData(TitleScreenModeValue, a);
    a = 0x06;
    writeData(VictoryModeValue, a);
    a >>= 1;
    writeData(GameModeValue, a);
    a = 0x00;
    JSR(ImposeGravity, 224);
    x = M(BulletBill_FrenzyVar);
    a = M(Misc_Y_Speed + x);
    compare(a, 0x05);
    if (!z)
        goto RunJCSubs;
    ++M(Misc_State + x);

RunJCSubs:
    JSR(RelativeMiscPosition, 225);
    JSR(GetMiscOffscreenBits, 226);
    JSR(GetMiscBoundBox, 227);
    JSR(JCoinGfxHandler, 228);

MiscLoopBack:
    --x;
    if (!n)
        goto MiscLoop;
    goto Return;
    // ;-------------------------------------------------------------------------------------

GiveOneCoin:
    a = 0x01;
    writeData(0x0139, a);
    x = M(CurrentPlayer);
    y = M(CoinTallyOffsets + x);
    JSR(DigitsMathRoutine, 229);
    ++M(CoinTally);
    a = M(CoinTally);
    compare(a, 0x64);
    if (!z)
        goto CoinPoints;
    a = 0x00;
    writeData(CoinTally, a);
    ++M(NumberofLives);
    a = 0x40;
    writeData(Square2SoundQueue, a);

CoinPoints:
    a = 0x02;
    writeData(0x0138, a);

AddToScore:
    x = M(CurrentPlayer);
    y = M(ScoreOffsets + x);
    JSR(DigitsMathRoutine, 230);

GetSBNybbles:
    y = M(CurrentPlayer);
    a = M(StatusBarNybbles + y);

UpdateNumber:
    JSR(PrintStatusBarNumbers, 231);
    y = M(VRAM_Buffer1_Offset);
    a = M(0x02fb + y);
    if (!z)
        goto NoZSup;
    a = 0x24;
    writeData(0x02fb + y, a);

NoZSup:
    x = M(BulletBill_FrenzyVar);
    goto Return;
    // ;-------------------------------------------------------------------------------------

SetupPowerUp:
    a = 0x2e;
    writeData(0x001b, a);
    a = M(Block_PageLoc + x);
    writeData(0x0073, a);
    a = M(Block_X_Position + x);
    writeData(0x008c, a);
    a = 0x01;
    writeData(0x00bb, a);
    a = M(Block_Y_Position + x);
    c = 1;
    a -= 0x08;
    writeData(0x00d4, a);

PwrUpJmp:
    a = 0x01;
    writeData(0x0023, a);
    writeData(FlyingCheepCheep, a);
    a = 0x03;
    writeData(0x049f, a);
    a = M(PowerUpType);
    compare(a, 0x02);
    if (c)
        goto PutBehind;
    a = M(PlayerStatus);
    compare(a, 0x02);
    if (!c)
        goto StrType;
    a >>= 1;

StrType:
    writeData(PowerUpType, a);

PutBehind:
    a = 0x20;
    writeData(0x03ca, a);
    a = 0x02;
    writeData(Square2SoundQueue, a);
    goto Return;
    // ;-------------------------------------------------------------------------------------

PowerUpObjHandler:
    x = 0x05;
    writeData(BulletBill_FrenzyVar, x);
    a = M(0x0023);
    if (z)
        goto ExitPUp;
    a <<= 1;
    if (!c)
        goto GrowThePowerUp;
    a = M(TimerControl);
    if (!z)
        goto RunPUSubs;
    a = M(PowerUpType);
    if (z)
        goto ShroomM;
    compare(a, 0x03);
    if (z)
        goto ShroomM;
    compare(a, 0x02);
    if (!z)
        goto RunPUSubs;
    JSR(MoveJumpingEnemy, 232);
    JSR(EnemyJump, 233);
    goto RunPUSubs;

ShroomM:
    JSR(MoveNormalEnemy, 234);
    JSR(EnemyToBGCollisionDet, 235);
    goto RunPUSubs;

GrowThePowerUp:
    a = M(TallEnemy);
    a &= 0x03;
    if (!z)
        goto ChkPUSte;
    --M(0x00d4);
    a = M(0x0023);
    ++M(0x0023);
    compare(a, 0x11);
    if (!c)
        goto ChkPUSte;
    a = 0x10;
    writeData(ExplosionGfxCounter + x, a);
    a = 0x80;
    writeData(0x0023, a);
    a <<= 1;
    writeData(0x03ca, a);
    a.rol();
    writeData(Enemy_MovingDir + x, a);

ChkPUSte:
    a = M(0x0023);
    compare(a, 0x06);
    if (!c)
        goto ExitPUp;

RunPUSubs:
    JSR(RelativeEnemyPosition, 236);
    JSR(GetEnemyOffscreenBits, 237);
    JSR(GetEnemyBoundBox, 238);
    JSR(DrawPowerUp, 239);
    JSR(PlayerEnemyCollision, 240);
    JSR(OffscreenBoundsCheck, 241);

ExitPUp:
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;These apply to all routines in this section unless otherwise noted:
    // ;$00 - used to store metatile from block buffer routine
    // ;$02 - used to store vertical high nybble offset from block buffer routine
    // ;$05 - used to store metatile stored in A at beginning of PlayerHeadCollision
    // ;$06-$07 - used as block buffer address indirect

PlayerHeadCollision:
    pha();
    a = 0x11;
    x = M(SprDataOffset_Ctrl);
    y = M(PlayerSize);
    if (!z)
        goto DBlockSte;
    a = 0x12;

DBlockSte:
    writeData(Block_State + x, a);
    JSR(DestroyBlockMetatile, 242);
    x = M(SprDataOffset_Ctrl);
    a = M(VictoryModeValue);
    writeData(Block_Orig_YPos + x, a);
    y = a;
    a = M(World7);
    writeData(Block_BBuf_Low + x, a);
    a = M(W(World7) + y);
    JSR(BlockBumpedChk, 243);
    writeData(TitleScreenModeValue, a);
    y = M(PlayerSize);
    if (!z)
        goto ChkBrick;
    a = y;

ChkBrick:
    if (!c)
        goto PutMTileB;
    y = 0x11;
    writeData(Block_State + x, y);
    a = 0xc4;
    y = M(TitleScreenModeValue);
    compare(y, 0x58);
    if (z)
        goto StartBTmr;
    compare(y, 0x5d);
    if (!z)
        goto PutMTileB;

StartBTmr:
    a = M(BrickCoinTimerFlag);
    if (!z)
        goto ContBTmr;
    a = 0x0b;
    writeData(BrickCoinTimer, a);
    ++M(BrickCoinTimerFlag);

ContBTmr:
    a = M(BrickCoinTimer);
    if (!z)
        goto PutOldMT;
    y = 0xc4;

PutOldMT:
    a = y;

PutMTileB:
    writeData(Block_Metatile + x, a);
    JSR(InitBlock_XY_Pos, 244);
    y = M(VictoryModeValue);
    a = 0x23;
    writeData(W(World7) + y, a);
    a = 0x10;
    writeData(BlockBounceTimer, a);
    pla();
    writeData(World6, a);
    y = 0x00;
    a = M(CrouchingFlag);
    if (!z)
        goto SmallBP;
    a = M(PlayerSize);
    if (z)
        goto BigBP;

SmallBP:
    ++y;

BigBP:
    a = M(Player_Y_Position);
    c = 0;
    a += M(BlockYPosAdderData + y);
    a &= 0xf0;
    writeData(Block_Y_Position + x, a);
    y = M(Block_State + x);
    compare(y, 0x11);
    if (z)
        goto Unbreak;
    JSR(BrickShatter, 245);
    goto InvOBit;

Unbreak:
    JSR(BumpBlock, 246);

InvOBit:
    a = M(SprDataOffset_Ctrl);
    a ^= 0x01;
    writeData(SprDataOffset_Ctrl, a);
    goto Return;
    // ;--------------------------------

InitBlock_XY_Pos:
    a = M(Player_X_Position);
    c = 0;
    a += 0x08;
    a &= 0xf0;
    writeData(Block_X_Position + x, a);
    a = M(Player_PageLoc);
    a += 0x00;
    writeData(Block_PageLoc + x, a);
    writeData(Block_PageLoc2 + x, a);
    a = M(Player_Y_HighPos);
    writeData(Block_Y_HighPos + x, a);
    goto Return;
    // ;--------------------------------

BumpBlock:
    JSR(CheckTopOfBlock, 247);
    a = 0x02;
    writeData(Square1SoundQueue, a);
    a = 0x00;
    writeData(Block_X_Speed + x, a);
    writeData(Block_Y_MoveForce + x, a);
    writeData(Player_Y_Speed, a);
    a = 0xfe;
    writeData(Block_Y_Speed + x, a);
    a = M(World6);
    JSR(BlockBumpedChk, 248);
    if (!c)
        goto ExitBlockChk;
    a = y;
    compare(a, 0x09);
    if (!c)
        goto loc_BDBD;
    a -= 0x05;
loc_BDBD:
    switch (a) {
        case 0: goto MushFlowerBlock;
        case 1: goto CoinBlock;
        case 2: goto CoinBlock;
        case 3: goto ExtraLifeMushBlock;
        case 4: goto MushFlowerBlock;
        case 5: goto VineBlock;
        case 6: goto StarBlock;
        case 7: goto CoinBlock;
        case 8: goto ExtraLifeMushBlock;
    }

MushFlowerBlock:
    a = 0x00;
    bit(M(0x02a9));
    goto loc_BDD7;

StarBlock:
    a = 0x02;
loc_BDD7:
    bit(M(0x03a9));
    goto loc_BDDA;

ExtraLifeMushBlock:
    a = 0x03;
loc_BDDA:
    writeData(PowerUpType, a);
    goto SetupPowerUp;

VineBlock:
    x = 0x05;
    y = M(SprDataOffset_Ctrl);
    JSR(Setup_Vine, 249);

ExitBlockChk:
    goto Return;
    // ;--------------------------------
    // ;these two sets are functionally identical, but look different

BlockBumpedChk:
    y = 0x0d;

BumpChkLoop:
    compare(a, M(BrickQBlockMetatiles + y));
    if (z)
        goto MatchBump;
    --y;
    if (!n)
        goto BumpChkLoop;
    c = 0;

MatchBump:
    goto Return;
    // ;--------------------------------

BrickShatter:
    JSR(CheckTopOfBlock, 250);
    a = 0x01;
    writeData(Block_RepFlag + x, a);
    writeData(NoiseSoundQueue, a);
    JSR(SpawnBrickChunks, 251);
    a = 0xfe;
    writeData(Player_Y_Speed, a);
    a = 0x05;
    writeData(0x0139, a);
    JSR(AddToScore, 252);
    x = M(SprDataOffset_Ctrl);
    goto Return;
    // ;--------------------------------

CheckTopOfBlock:
    x = M(SprDataOffset_Ctrl);
    y = M(VictoryModeValue);
    if (z)
        goto TopEx;
    a = y;
    c = 1;
    a -= 0x10;
    writeData(VictoryModeValue, a);
    y = a;
    a = M(W(World7) + y);
    compare(a, 0xc2);
    if (!z)
        goto TopEx;
    a = 0x00;
    writeData(W(World7) + y, a);
    JSR(RemoveCoin_Axe, 253);
    x = M(SprDataOffset_Ctrl);
    JSR(SetupJumpCoin, 254);

TopEx:
    goto Return;
    // ;--------------------------------

SpawnBrickChunks:
    a = M(Block_X_Position + x);
    writeData(Block_Orig_XPos + x, a);
    a = 0xf0;
    writeData(Block_X_Speed + x, a);
    writeData(0x0062 + x, a);
    a = 0xfa;
    writeData(Block_Y_Speed + x, a);
    a = 0xfc;
    writeData(0x00aa + x, a);
    a = 0x00;
    writeData(Block_Y_MoveForce + x, a);
    writeData(0x043e + x, a);
    a = M(Block_PageLoc + x);
    writeData(0x0078 + x, a);
    a = M(Block_X_Position + x);
    writeData(0x0091 + x, a);
    a = M(Block_Y_Position + x);
    c = 0;
    a += 0x08;
    writeData(0x00d9 + x, a);
    a = 0xfa;
    writeData(Block_Y_Speed + x, a);
    goto Return;
    // ;-------------------------------------------------------------------------------------

BlockObjectsCore:
    a = M(Block_State + x);
    if (z)
        goto UpdSte;
    a &= 0x0f;
    pha();
    y = a;
    a = x;
    c = 0;
    a += 0x09;
    x = a;
    --y;
    if (z)
        goto BouncingBlockHandler;
    JSR(ImposeGravityBlock, 255);
    JSR(MoveObjectHorizontally, 256);
    a = x;
    c = 0;
    a += 0x02;
    x = a;
    JSR(ImposeGravityBlock, 257);
    JSR(MoveObjectHorizontally, 258);
    x = M(BulletBill_FrenzyVar);
    JSR(RelativeBlockPosition, 259);
    JSR(GetBlockOffscreenBits, 260);
    JSR(DrawBrickChunks, 261);
    pla();
    y = M(Block_Y_HighPos + x);
    if (z)
        goto UpdSte;
    pha();
    a = 0xf0;
    compare(a, M(0x00d9 + x));
    if (c)
        goto ChkTop;
    writeData(0x00d9 + x, a);

ChkTop:
    a = M(Block_Y_Position + x);
    compare(a, 0xf0);
    pla();
    if (!c)
        goto UpdSte;
    if (c)
        goto KillBlock;

BouncingBlockHandler:
    JSR(ImposeGravityBlock, 262);
    x = M(BulletBill_FrenzyVar);
    JSR(RelativeBlockPosition, 263);
    JSR(GetBlockOffscreenBits, 264);
    JSR(DrawBlock, 265);
    a = M(Block_Y_Position + x);
    a &= 0x0f;
    compare(a, 0x05);
    pla();
    if (c)
        goto UpdSte;
    a = 0x01;
    writeData(Block_RepFlag + x, a);

KillBlock:
    a = 0x00;

UpdSte:
    writeData(Block_State + x, a);
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$02 - used to store offset to block buffer
    // ;$06-$07 - used to store block buffer address

BlockObjMT_Updater:
    x = 0x01;

UpdateLoop:
    writeData(BulletBill_FrenzyVar, x);
    a = M(VRAM_Buffer1);
    if (!z)
        goto NextBUpd;
    a = M(Block_RepFlag + x);
    if (z)
        goto NextBUpd;
    a = M(Block_BBuf_Low + x);
    writeData(World7, a);
    a = 0x05;
    writeData(World8, a);
    a = M(Block_Orig_YPos + x);
    writeData(VictoryModeValue, a);
    y = a;
    a = M(Block_Metatile + x);
    writeData(W(World7) + y, a);
    JSR(ReplaceBlockMetatile, 266);
    a = 0x00;
    writeData(Block_RepFlag + x, a);

NextBUpd:
    --x;
    if (!n)
        goto UpdateLoop;
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$00 - used to store high nybble of horizontal speed as adder
    // ;$01 - used to store low nybble of horizontal speed
    // ;$02 - used to store adder to page location

MoveEnemyHorizontally:
    ++x;
    JSR(MoveObjectHorizontally, 267);
    x = M(BulletBill_FrenzyVar);
    goto Return;

MovePlayerHorizontally:
    a = M(JumpspringAnimCtrl);
    if (!z)
        goto ExXMove;
    x = a;

MoveObjectHorizontally:
    a = M(Player_X_Speed + x);
    a <<= 1;
    a <<= 1;
    a <<= 1;
    a <<= 1;
    writeData(GameModeValue, a);
    a = M(Player_X_Speed + x);
    a >>= 1;
    a >>= 1;
    a >>= 1;
    a >>= 1;
    compare(a, 0x08);
    if (!c)
        goto SaveXSpd;
    a |= 0xf0;

SaveXSpd:
    writeData(TitleScreenModeValue, a);
    y = 0x00;
    compare(a, 0x00);
    if (!n)
        goto UseAdder;
    --y;

UseAdder:
    writeData(VictoryModeValue, y);
    a = M(SprObject_X_MoveForce + x);
    c = 0;
    a += M(GameModeValue);
    writeData(SprObject_X_MoveForce + x, a);
    a = 0x00;
    a.rol();
    pha();
    a.ror();
    a = M(Player_X_Position + x);
    a += M(TitleScreenModeValue);
    writeData(Player_X_Position + x, a);
    a = M(Player_PageLoc + x);
    a += M(VictoryModeValue);
    writeData(Player_PageLoc + x, a);
    pla();
    c = 0;
    a += M(TitleScreenModeValue);

ExXMove:
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$00 - used for downward force
    // ;$01 - used for upward force
    // ;$02 - used for maximum vertical speed

MovePlayerVertically:
    x = 0x00;
    a = M(TimerControl);
    if (!z)
        goto NoJSChk;
    a = M(JumpspringAnimCtrl);
    if (!z)
        goto ExXMove;

NoJSChk:
    a = M(VerticalForce);
    writeData(TitleScreenModeValue, a);
    a = 0x04;
    goto ImposeGravitySprObj;
    // ;--------------------------------

MoveD_EnemyVertically:
    y = 0x3d;
    a = M(Enemy_State + x);
    compare(a, 0x05);
    if (!z)
        goto ContVMove;

MoveFallingPlatform:
    y = 0x20;

ContVMove:
    goto SetHiMax;
    // ;--------------------------------

MoveRedPTroopaDown:
    y = 0x00;
    goto MoveRedPTroopa;

MoveRedPTroopaUp:
    y = 0x01;

MoveRedPTroopa:
    ++x;
    a = 0x03;
    writeData(TitleScreenModeValue, a);
    a = 0x06;
    writeData(GameModeValue, a);
    a = 0x02;
    writeData(VictoryModeValue, a);
    a = y;
    goto RedPTroopaGrav;
    // ;--------------------------------

MoveDropPlatform:
    y = 0x7f;
    if (!z)
        goto SetMdMax;

MoveEnemySlowVert:
    y = 0x0f;

SetMdMax:
    a = 0x02;
    if (!z)
        goto SetXMoveAmt;
    // ;--------------------------------

MoveJ_EnemyVertically:
    y = 0x1c;

SetHiMax:
    a = 0x03;

SetXMoveAmt:
    writeData(TitleScreenModeValue, y);
    ++x;
    JSR(ImposeGravitySprObj, 268);
    x = M(BulletBill_FrenzyVar);
    goto Return;
    // ;--------------------------------

ResidualGravityCode:
    y = 0x00;
    bit(M(0x01a0));
    goto loc_BFA6;

ImposeGravityBlock:
    y = 0x01;
loc_BFA6:
    a = 0x50;
    writeData(TitleScreenModeValue, a);
    a = M(MaxSpdBlockData + y);

ImposeGravitySprObj:
    writeData(VictoryModeValue, a);
    a = 0x00;
    goto ImposeGravity;
    // ;--------------------------------

MovePlatformDown:
    a = 0x00;
    bit(M(0x01a9));
    goto loc_BFB9;

MovePlatformUp:
    a = 0x01;
loc_BFB9:
    pha();
    y = M(Fireworks + x);
    ++x;
    a = 0x05;
    compare(y, 0x29);
    if (!z)
        goto SetDplSpd;
    a = 0x09;

SetDplSpd:
    writeData(TitleScreenModeValue, a);
    a = 0x0a;
    writeData(GameModeValue, a);
    a = 0x03;
    writeData(VictoryModeValue, a);
    pla();
    y = a;

RedPTroopaGrav:
    JSR(ImposeGravity, 269);
    x = M(BulletBill_FrenzyVar);
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$00 - used for downward force
    // ;$01 - used for upward force
    // ;$07 - used as adder for vertical position

ImposeGravity:
    pha();
    a = M(Player_YMF_Dummy + x);
    c = 0;
    a += M(Player_Y_MoveForce + x);
    writeData(Player_YMF_Dummy + x, a);
    y = 0x00;
    a = M(Player_Y_Speed + x);
    if (!n)
        goto AlterYP;
    --y;

AlterYP:
    writeData(World8, y);
    a += M(Player_Y_Position + x);
    writeData(Player_Y_Position + x, a);
    a = M(Player_Y_HighPos + x);
    a += M(World8);
    writeData(Player_Y_HighPos + x, a);
    a = M(Player_Y_MoveForce + x);
    c = 0;
    a += M(TitleScreenModeValue);
    writeData(Player_Y_MoveForce + x, a);
    a = M(Player_Y_Speed + x);
    a += 0x00;
    writeData(Player_Y_Speed + x, a);
    compare(a, M(VictoryModeValue));
    if (n)
        goto ChkUpM;
    a = M(Player_Y_MoveForce + x);
    compare(a, 0x80);
    if (!c)
        goto ChkUpM;
    a = M(VictoryModeValue);
    writeData(Player_Y_Speed + x, a);
    a = 0x00;
    writeData(Player_Y_MoveForce + x, a);

ChkUpM:
    pla();
    if (z)
        goto ExVMove;
    a = M(VictoryModeValue);
    a ^= 0xff;
    y = a;
    ++y;
    writeData(World8, y);
    a = M(Player_Y_MoveForce + x);
    c = 1;
    a -= M(GameModeValue);
    writeData(Player_Y_MoveForce + x, a);
    a = M(Player_Y_Speed + x);
    a -= 0x00;
    writeData(Player_Y_Speed + x, a);
    compare(a, M(World8));
    if (!n)
        goto ExVMove;
    a = M(Player_Y_MoveForce + x);
    compare(a, 0x80);
    if (c)
        goto ExVMove;
    a = M(World8);
    writeData(Player_Y_Speed + x, a);
    a = 0xff;
    writeData(Player_Y_MoveForce + x, a);

ExVMove:
    goto Return;
    // ;-------------------------------------------------------------------------------------

EnemiesAndLoopsCore:
    a = M(RedParatroopa + x);
    pha();
    a <<= 1;
    if (c)
        goto ChkBowserF;
    pla();
    if (z)
        goto ChkAreaTsk;
    goto RunEnemyObjectsCore;

ChkAreaTsk:
    a = M(AreaParserTaskNum);
    a &= 0x07;
    compare(a, 0x07);
    if (z)
        goto ExitELCore;
    goto ProcLoopCommand;

ChkBowserF:
    pla();
    a &= 0x0f;
    y = a;
    a = M(RedParatroopa + y);
    if (!z)
        goto ExitELCore;
    writeData(RedParatroopa + x, a);

ExitELCore:
    goto Return;
    // ;--------------------------------
    // ;loop command data

ExecGameLoopback:
    a = M(Player_PageLoc);
    c = 1;
    a -= 0x04;
    writeData(Player_PageLoc, a);
    a = M(CurrentPageLoc);
    c = 1;
    a -= 0x04;
    writeData(CurrentPageLoc, a);
    a = M(ScreenLeft_PageLoc);
    c = 1;
    a -= 0x04;
    writeData(ScreenLeft_PageLoc, a);
    a = M(ScreenRight_PageLoc);
    c = 1;
    a -= 0x04;
    writeData(ScreenRight_PageLoc, a);
    a = M(AreaObjectPageLoc);
    c = 1;
    a -= 0x04;
    writeData(AreaObjectPageLoc, a);
    a = 0x00;
    writeData(EnemyObjectPageSel, a);
    writeData(AreaObjectPageSel, a);
    writeData(EnemyDataOffset, a);
    writeData(EnemyObjectPageLoc, a);
    a = M(AreaDataOfsLoopback + y);
    writeData(AreaDataOffset, a);
    goto Return;

ProcLoopCommand:
    a = M(LoopCommand);
    if (z)
        goto ChkEnemyFrenzy;
    a = M(CurrentColumnPos);
    if (!z)
        goto ChkEnemyFrenzy;
    y = 0x0b;

FindLoop:
    --y;
    if (n)
        goto ChkEnemyFrenzy;
    a = M(WorldNumber);
    compare(a, M(LoopCmdWorldNumber + y));
    if (!z)
        goto FindLoop;
    a = M(CurrentPageLoc);
    compare(a, M(LoopCmdPageNumber + y));
    if (!z)
        goto FindLoop;
    a = M(Player_Y_Position);
    compare(a, M(LoopCmdYPosition + y));
    if (!z)
        goto WrongChk;
    a = M(Player_State);
    compare(a, 0x00);
    if (!z)
        goto WrongChk;
    a = M(WorldNumber);
    compare(a, 0x06);
    if (!z)
        goto InitMLp;
    ++M(MultiLoopCorrectCntr);

IncMLoop:
    ++M(MultiLoopPassCntr);
    a = M(MultiLoopPassCntr);
    compare(a, 0x03);
    if (!z)
        goto InitLCmd;
    a = M(MultiLoopCorrectCntr);
    compare(a, 0x03);
    if (z)
        goto InitMLp;
    if (!z)
        goto DoLpBack;

WrongChk:
    a = M(WorldNumber);
    compare(a, 0x06);
    if (z)
        goto IncMLoop;

DoLpBack:
    JSR(ExecGameLoopback, 270);
    JSR(KillAllEnemies, 271);

InitMLp:
    a = 0x00;
    writeData(MultiLoopPassCntr, a);
    writeData(MultiLoopCorrectCntr, a);

InitLCmd:
    a = 0x00;
    writeData(LoopCommand, a);
    // ;--------------------------------

ChkEnemyFrenzy:
    a = M(EnemyFrenzyQueue);
    if (z)
        goto ProcessEnemyData;
    writeData(Fireworks + x, a);
    a = 0x01;
    writeData(RedParatroopa + x, a);
    a = 0x00;
    writeData(Enemy_State + x, a);
    writeData(EnemyFrenzyQueue, a);
    goto InitEnemyObject;
    // ;--------------------------------
    // ;$06 - used to hold page location of extended right boundary
    // ;$07 - used to hold high nybble of position of extended right boundary

ProcessEnemyData:
    y = M(EnemyDataOffset);
    a = M(W(EnemyDataLow) + y);
    compare(a, 0xff);
    if (!z)
        goto CheckEndofBuffer;
    goto CheckFrenzyBuffer;

CheckEndofBuffer:
    a &= 0x0f;
    compare(a, 0x0e);
    if (z)
        goto CheckRightBounds;
    compare(x, 0x05);
    if (!c)
        goto CheckRightBounds;
    ++y;
    a = M(W(EnemyDataLow) + y);
    a &= 0x3f;
    compare(a, 0x2e);
    if (z)
        goto CheckRightBounds;
    goto Return;

CheckRightBounds:
    a = M(ScreenRight_X_Pos);
    c = 0;
    a += 0x30;
    a &= 0xf0;
    writeData(World8, a);
    a = M(ScreenRight_PageLoc);
    a += 0x00;
    writeData(World7, a);
    y = M(EnemyDataOffset);
    ++y;
    a = M(W(EnemyDataLow) + y);
    a <<= 1;
    if (!c)
        goto CheckPageCtrlRow;
    a = M(EnemyObjectPageSel);
    if (!z)
        goto CheckPageCtrlRow;
    ++M(EnemyObjectPageSel);
    ++M(EnemyObjectPageLoc);

CheckPageCtrlRow:
    --y;
    a = M(W(EnemyDataLow) + y);
    a &= 0x0f;
    compare(a, 0x0f);
    if (!z)
        goto PositionEnemyObj;
    a = M(EnemyObjectPageSel);
    if (!z)
        goto PositionEnemyObj;
    ++y;
    a = M(W(EnemyDataLow) + y);
    a &= 0x3f;
    writeData(EnemyObjectPageLoc, a);
    ++M(EnemyDataOffset);
    ++M(EnemyDataOffset);
    ++M(EnemyObjectPageSel);
    goto ProcLoopCommand;

PositionEnemyObj:
    a = M(EnemyObjectPageLoc);
    writeData(Enemy_PageLoc + x, a);
    a = M(W(EnemyDataLow) + y);
    a &= 0xf0;
    writeData(Enemy_X_Position + x, a);
    compare(a, M(ScreenRight_X_Pos));
    a = M(Enemy_PageLoc + x);
    a -= M(ScreenRight_PageLoc);
    if (c)
        goto CheckRightExtBounds;
    a = M(W(EnemyDataLow) + y);
    a &= 0x0f;
    compare(a, 0x0e);
    if (z)
        goto ParseRow0e;
    goto CheckThreeBytes;

CheckRightExtBounds:
    a = M(World8);
    compare(a, M(Enemy_X_Position + x));
    a = M(World7);
    a -= M(Enemy_PageLoc + x);
    if (!c)
        goto CheckFrenzyBuffer;
    a = 0x01;
    writeData(Enemy_Y_HighPos + x, a);
    a = M(W(EnemyDataLow) + y);
    a <<= 1;
    a <<= 1;
    a <<= 1;
    a <<= 1;
    writeData(Enemy_Y_Position + x, a);
    compare(a, 0xe0);
    if (z)
        goto ParseRow0e;
    ++y;
    a = M(W(EnemyDataLow) + y);
    a &= 0x40;
    if (z)
        goto CheckForEnemyGroup;
    a = M(SecondaryHardMode);
    if (z)
        goto Inc2B;

CheckForEnemyGroup:
    a = M(W(EnemyDataLow) + y);
    a &= 0x3f;
    compare(a, 0x37);
    if (!c)
        goto BuzzyBeetleMutate;
    compare(a, 0x3f);
    if (!c)
        goto DoGroup;

BuzzyBeetleMutate:
    compare(a, 0x06);
    if (!z)
        goto StrID;
    y = M(PrimaryHardMode);
    if (z)
        goto StrID;
    a = 0x02;

StrID:
    writeData(Fireworks + x, a);
    a = 0x01;
    writeData(RedParatroopa + x, a);
    JSR(InitEnemyObject, 272);
    a = M(RedParatroopa + x);
    if (!z)
        goto Inc2B;
    goto Return;

CheckFrenzyBuffer:
    a = M(EnemyFrenzyBuffer);
    if (!z)
        goto StrFre;
    a = M(VineFlagOffset);
    compare(a, 0x01);
    if (!z)
        goto ExEPar;
    a = 0x2f;

StrFre:
    writeData(Fireworks + x, a);

InitEnemyObject:
    a = 0x00;
    writeData(Enemy_State + x, a);
    JSR(CheckpointEnemyID, 273);

ExEPar:
    goto Return;

DoGroup:
    goto HandleGroupEnemies;

ParseRow0e:
    ++y;
    ++y;
    a = M(W(EnemyDataLow) + y);
    a >>= 1;
    a >>= 1;
    a >>= 1;
    a >>= 1;
    a >>= 1;
    compare(a, M(WorldNumber));
    if (!z)
        goto NotUse;
    --y;
    a = M(W(EnemyDataLow) + y);
    writeData(AreaPointer, a);
    ++y;
    a = M(W(EnemyDataLow) + y);
    a &= 0x1f;
    writeData(EntrancePage, a);

NotUse:
    goto Inc3B;

CheckThreeBytes:
    y = M(EnemyDataOffset);
    a = M(W(EnemyDataLow) + y);
    a &= 0x0f;
    compare(a, 0x0e);
    if (!z)
        goto Inc2B;

Inc3B:
    ++M(EnemyDataOffset);

Inc2B:
    ++M(EnemyDataOffset);
    ++M(EnemyDataOffset);
    a = 0x00;
    writeData(EnemyObjectPageSel, a);
    x = M(BulletBill_FrenzyVar);
    goto Return;

CheckpointEnemyID:
    a = M(Fireworks + x);
    compare(a, 0x15);
    if (c)
        goto InitEnemyRoutines;
    y = a;
    a = M(Enemy_Y_Position + x);
    a += 0x08;
    writeData(Enemy_Y_Position + x, a);
    a = 0x01;
    writeData(EnemyOffscrBitsMasked + x, a);
    a = y;

InitEnemyRoutines:
    switch (a) {
        case 0: goto InitNormalEnemy;
        case 1: goto InitNormalEnemy;
        case 2: goto InitNormalEnemy;
        case 3: goto InitRedKoopa;
        case 4: goto NoInitCode;
        case 5: goto InitHammerBro;
        case 6: goto InitGoomba;
        case 7: goto InitBloober;
        case 8: goto InitBulletBill;
        case 9: goto NoInitCode;
        case 10: goto InitCheepCheep;
        case 11: goto InitCheepCheep;
        case 12: goto InitPodoboo;
        case 13: goto InitPiranhaPlant;
        case 14: goto InitJumpGPTroopa;
        case 15: goto InitRedPTroopa;
        case 16: goto InitHorizFlySwimEnemy;
        case 17: goto InitLakitu;
        case 18: goto InitEnemyFrenzy;
        case 19: goto NoInitCode;
        case 20: goto InitEnemyFrenzy;
        case 21: goto InitEnemyFrenzy;
        case 22: goto InitEnemyFrenzy;
        case 23: goto InitEnemyFrenzy;
        case 24: goto EndFrenzy;
        case 25: goto NoInitCode;
        case 26: goto NoInitCode;
        case 27: goto InitShortFirebar;
        case 28: goto InitShortFirebar;
        case 29: goto InitShortFirebar;
        case 30: goto InitShortFirebar;
        case 31: goto InitLongFirebar;
        case 32: goto NoInitCode;
        case 33: goto NoInitCode;
        case 34: goto NoInitCode;
        case 35: goto NoInitCode;
        case 36: goto InitBalPlatform;
        case 37: goto InitVertPlatform;
        case 38: goto LargeLiftUp;
        case 39: goto LargeLiftDown;
        case 40: goto InitHoriPlatform;
        case 41: goto InitDropPlatform;
        case 42: goto InitHoriPlatform;
        case 43: goto PlatLiftUp;
        case 44: goto PlatLiftDown;
        case 45: goto InitBowser;
        case 46: goto PwrUpJmp;
        case 47: goto Setup_Vine;
        case 48: goto NoInitCode;
        case 49: goto NoInitCode;
        case 50: goto NoInitCode;
        case 51: goto NoInitCode;
        case 52: goto NoInitCode;
        case 53: goto InitRetainerObj;
        case 54: goto EndOfEnemyInitCode;
    }

NoInitCode:
    goto Return;
    // ;--------------------------------

InitGoomba:
    JSR(InitNormalEnemy, 274);
    goto SmallBBox;
    // ;--------------------------------

InitPodoboo:
    a = 0x02;
    writeData(Enemy_Y_HighPos + x, a);
    writeData(Enemy_Y_Position + x, a);
    a >>= 1;
    writeData(EnemyIntervalTimer + x, a);
    a >>= 1;
    writeData(Enemy_State + x, a);
    goto SmallBBox;
    // ;--------------------------------

InitRetainerObj:
    a = 0xb8;
    writeData(Enemy_Y_Position + x, a);
    goto Return;
    // ;--------------------------------

InitNormalEnemy:
    y = 0x01;
    a = M(PrimaryHardMode);
    if (!z)
        goto GetESpd;
    --y;

GetESpd:
    a = M(NormalXSpdData + y);

SetESpd:
    writeData(ExplosionGfxCounter + x, a);
    goto TallBBox;
    // ;--------------------------------

InitRedKoopa:
    JSR(InitNormalEnemy, 275);
    a = 0x01;
    writeData(Enemy_State + x, a);
    goto Return;
    // ;--------------------------------

InitHammerBro:
    a = 0x00;
    writeData(HammerThrowingTimer + x, a);
    writeData(ExplosionGfxCounter + x, a);
    y = M(SecondaryHardMode);
    a = M(HBroWalkingTimerData + y);
    writeData(EnemyIntervalTimer + x, a);
    a = 0x0b;
    goto SetBBox;
    // ;--------------------------------

InitHorizFlySwimEnemy:
    a = 0x00;
    goto SetESpd;
    // ;--------------------------------

InitBloober:
    a = 0x00;
    writeData(ExplosionGfxCounter + x, a);

SmallBBox:
    a = 0x09;
    if (!z)
        goto SetBBox;
    // ;--------------------------------

InitRedPTroopa:
    y = 0x30;
    a = M(Enemy_Y_Position + x);
    writeData(RedPTroopaOrigXPos + x, a);
    if (!n)
        goto GetCent;
    y = 0xe0;

GetCent:
    a = y;
    a += M(Enemy_Y_Position + x);
    writeData(ExplosionGfxCounter + x, a);

TallBBox:
    a = 0x03;

SetBBox:
    writeData(Enemy_BoundBoxCtrl + x, a);
    a = 0x02;
    writeData(Enemy_MovingDir + x, a);

InitVStf:
    a = 0x00;
    writeData(ExplosionTimerCounter + x, a);
    writeData(PiranhaPlantDownYPos + x, a);
    goto Return;
    // ;--------------------------------

InitBulletBill:
    a = 0x02;
    writeData(Enemy_MovingDir + x, a);
    a = 0x09;
    writeData(Enemy_BoundBoxCtrl + x, a);
    goto Return;
    // ;--------------------------------

InitCheepCheep:
    JSR(SmallBBox, 276);
    a = M(PseudoRandomBitReg + x);
    a &= 0x10;
    writeData(ExplosionGfxCounter + x, a);
    a = M(Enemy_Y_Position + x);
    writeData(PiranhaPlantDownYPos + x, a);
    goto Return;
    // ;--------------------------------

InitLakitu:
    a = M(EnemyFrenzyBuffer);
    if (!z)
        goto KillLakitu;

SetupLakitu:
    a = 0x00;
    writeData(LakituReappearTimer, a);
    JSR(InitHorizFlySwimEnemy, 277);
    goto TallBBox2;

KillLakitu:
    goto EraseEnemyObject;
    // ;--------------------------------
    // ;$01-$03 - used to hold pseudorandom difference adjusters

LakituAndSpinyHandler:
    a = M(FrenzyEnemyTimer);
    if (!z)
        goto ExLSHand;
    compare(x, 0x05);
    if (c)
        goto ExLSHand;
    a = 0x80;
    writeData(FrenzyEnemyTimer, a);
    y = 0x04;

ChkLak:
    a = M(Fireworks + y);
    compare(a, 0x11);
    if (z)
        goto CreateSpiny;
    --y;
    if (!n)
        goto ChkLak;
    ++M(LakituReappearTimer);
    a = M(LakituReappearTimer);
    compare(a, 0x07);
    if (!c)
        goto ExLSHand;
    x = 0x04;

ChkNoEn:
    a = M(RedParatroopa + x);
    if (z)
        goto CreateL;
    --x;
    if (!n)
        goto ChkNoEn;
    if (n)
        goto RetEOfs;

CreateL:
    a = 0x00;
    writeData(Enemy_State + x, a);
    a = 0x11;
    writeData(Fireworks + x, a);
    JSR(SetupLakitu, 278);
    a = 0x20;
    JSR(PutAtRightExtent, 279);

RetEOfs:
    x = M(BulletBill_FrenzyVar);

ExLSHand:
    goto Return;
    // ;--------------------------------

CreateSpiny:
    a = M(Player_Y_Position);
    compare(a, 0x2c);
    if (!c)
        goto ExLSHand;
    a = M(Enemy_State + y);
    if (!z)
        goto ExLSHand;
    a = M(Enemy_PageLoc + y);
    writeData(Enemy_PageLoc + x, a);
    a = M(Enemy_X_Position + y);
    writeData(Enemy_X_Position + x, a);
    a = 0x01;
    writeData(Enemy_Y_HighPos + x, a);
    a = M(Enemy_Y_Position + y);
    c = 1;
    a -= 0x08;
    writeData(Enemy_Y_Position + x, a);
    a = M(PseudoRandomBitReg + x);
    a &= 0x03;
    y = a;
    x = 0x02;

DifLoop:
    a = M(PRDiffAdjustData + y);
    writeData(GameModeValue + x, a);
    ++y;
    ++y;
    ++y;
    ++y;
    --x;
    if (!n)
        goto DifLoop;
    x = M(BulletBill_FrenzyVar);
    JSR(PlayerLakituDiff, 280);
    y = M(Player_X_Speed);
    compare(y, 0x08);
    if (c)
        goto SetSpSpd;
    y = a;
    a = M(0x07a8 + x);
    a &= 0x03;
    if (z)
        goto UsePosv;
    a = y;
    a ^= 0xff;
    y = a;
    ++y;

UsePosv:
    a = y;

SetSpSpd:
    JSR(SmallBBox, 281);
    y = 0x02;
    writeData(ExplosionGfxCounter + x, a);
    compare(a, 0x00);
    if (n)
        goto SpinyRte;
    --y;

SpinyRte:
    writeData(Enemy_MovingDir + x, y);
    a = 0xfd;
    writeData(ExplosionTimerCounter + x, a);
    a = 0x01;
    writeData(RedParatroopa + x, a);
    a = 0x05;
    writeData(Enemy_State + x, a);

ChpChpEx:
    goto Return;
    // ;--------------------------------

InitLongFirebar:
    JSR(DuplicateEnemyObj, 282);

InitShortFirebar:
    a = 0x00;
    writeData(ExplosionGfxCounter + x, a);
    a = M(Fireworks + x);
    c = 1;
    a -= 0x1b;
    y = a;
    a = M(FirebarSpinSpdData + y);
    writeData(FirebarSpinSpeed + x, a);
    a = M(FirebarSpinDirData + y);
    writeData(FirebarSpinDirection + x, a);
    a = M(Enemy_Y_Position + x);
    c = 0;
    a += 0x04;
    writeData(Enemy_Y_Position + x, a);
    a = M(Enemy_X_Position + x);
    c = 0;
    a += 0x04;
    writeData(Enemy_X_Position + x, a);
    a = M(Enemy_PageLoc + x);
    a += 0x00;
    writeData(Enemy_PageLoc + x, a);
    goto TallBBox2;
    // ;--------------------------------
    // ;$00-$01 - used to hold pseudorandom bits

InitFlyingCheepCheep:
    a = M(FrenzyEnemyTimer);
    if (!z)
        goto ChpChpEx;
    JSR(SmallBBox, 283);
    a = M(0x07a8 + x);
    a &= 0x03;
    y = a;
    a = M(FlyCCTimerData + y);
    writeData(FrenzyEnemyTimer, a);
    y = 0x03;
    a = M(SecondaryHardMode);
    if (z)
        goto MaxCC;
    ++y;

MaxCC:
    writeData(TitleScreenModeValue, y);
    compare(x, M(TitleScreenModeValue));
    if (c)
        goto ChpChpEx;
    a = M(PseudoRandomBitReg + x);
    a &= 0x03;
    writeData(TitleScreenModeValue, a);
    writeData(GameModeValue, a);
    a = 0xfb;
    writeData(ExplosionTimerCounter + x, a);
    a = 0x00;
    y = M(Player_X_Speed);
    if (z)
        goto GSeed;
    a = 0x04;
    compare(y, 0x19);
    if (!c)
        goto GSeed;
    a <<= 1;

GSeed:
    pha();
    c = 0;
    a += M(TitleScreenModeValue);
    writeData(TitleScreenModeValue, a);
    a = M(0x07a8 + x);
    a &= 0x03;
    if (z)
        goto RSeed;
    a = M(0x07a9 + x);
    a &= 0x0f;
    writeData(TitleScreenModeValue, a);

RSeed:
    pla();
    c = 0;
    a += M(GameModeValue);
    y = a;
    a = M(FlyCCXSpeedData + y);
    writeData(ExplosionGfxCounter + x, a);
    a = 0x01;
    writeData(Enemy_MovingDir + x, a);
    a = M(Player_X_Speed);
    if (!z)
        goto D2XPos1;
    y = M(TitleScreenModeValue);
    a = y;
    a &= 0x02;
    if (z)
        goto D2XPos1;
    a = M(ExplosionGfxCounter + x);
    a ^= 0xff;
    c = 0;
    a += 0x01;
    writeData(ExplosionGfxCounter + x, a);
    ++M(Enemy_MovingDir + x);

D2XPos1:
    a = y;
    a &= 0x02;
    if (z)
        goto D2XPos2;
    a = M(Player_X_Position);
    c = 0;
    a += M(FlyCCXPositionData + y);
    writeData(Enemy_X_Position + x, a);
    a = M(Player_PageLoc);
    a += 0x00;
    goto FinCCSt;

D2XPos2:
    a = M(Player_X_Position);
    c = 1;
    a -= M(FlyCCXPositionData + y);
    writeData(Enemy_X_Position + x, a);
    a = M(Player_PageLoc);
    a -= 0x00;

FinCCSt:
    writeData(Enemy_PageLoc + x, a);
    a = 0x01;
    writeData(RedParatroopa + x, a);
    writeData(Enemy_Y_HighPos + x, a);
    a = 0xf8;
    writeData(Enemy_Y_Position + x, a);
    goto Return;
    // ;--------------------------------

InitBowser:
    JSR(DuplicateEnemyObj, 284);
    writeData(BowserFront_Offset, x);
    a = 0x00;
    writeData(BowserBodyControls, a);
    writeData(BridgeCollapseOffset, a);
    a = M(Enemy_X_Position + x);
    writeData(BowserOrigXPos, a);
    a = 0xdf;
    writeData(BowserFireBreathTimer, a);
    writeData(Enemy_MovingDir + x, a);
    a = 0x20;
    writeData(BowserFeetCounter, a);
    writeData(EnemyFrameTimer + x, a);
    a = 0x05;
    writeData(BowserHitPoints, a);
    a >>= 1;
    writeData(BowserMovementSpeed, a);
    goto Return;
    // ;--------------------------------

DuplicateEnemyObj:
    y = 0xff;

FSLoop:
    ++y;
    a = M(RedParatroopa + y);
    if (!z)
        goto FSLoop;
    writeData(DuplicateObj_Offset, y);
    a = x;
    a |= 0x80;
    writeData(RedParatroopa + y, a);
    a = M(Enemy_PageLoc + x);
    writeData(Enemy_PageLoc + y, a);
    a = M(Enemy_X_Position + x);
    writeData(Enemy_X_Position + y, a);
    a = 0x01;
    writeData(RedParatroopa + x, a);
    writeData(Enemy_Y_HighPos + y, a);
    a = M(Enemy_Y_Position + x);
    writeData(Enemy_Y_Position + y, a);

FlmEx:
    goto Return;
    // ;--------------------------------

InitBowserFlame:
    a = M(FrenzyEnemyTimer);
    if (!z)
        goto FlmEx;
    writeData(PiranhaPlantDownYPos + x, a);
    a = M(NoiseSoundQueue);
    a |= 0x02;
    writeData(NoiseSoundQueue, a);
    y = M(BowserFront_Offset);
    a = M(Fireworks + y);
    compare(a, 0x2d);
    if (z)
        goto SpawnFromMouth;
    JSR(SetFlameTimer, 285);
    c = 0;
    a += 0x20;
    y = M(SecondaryHardMode);
    if (z)
        goto SetFrT;
    c = 1;
    a -= 0x10;

SetFrT:
    writeData(FrenzyEnemyTimer, a);
    a = M(PseudoRandomBitReg + x);
    a &= 0x03;
    writeData(PiranhaPlantUpYPos + x, a);
    y = a;
    a = M(FlameYPosData + y);

PutAtRightExtent:
    writeData(Enemy_Y_Position + x, a);
    a = M(ScreenRight_X_Pos);
    c = 0;
    a += 0x20;
    writeData(Enemy_X_Position + x, a);
    a = M(ScreenRight_PageLoc);
    a += 0x00;
    writeData(Enemy_PageLoc + x, a);
    goto FinishFlame;

SpawnFromMouth:
    a = M(Enemy_X_Position + y);
    c = 1;
    a -= 0x0e;
    writeData(Enemy_X_Position + x, a);
    a = M(Enemy_PageLoc + y);
    writeData(Enemy_PageLoc + x, a);
    a = M(Enemy_Y_Position + y);
    c = 0;
    a += 0x08;
    writeData(Enemy_Y_Position + x, a);
    a = M(PseudoRandomBitReg + x);
    a &= 0x03;
    writeData(PiranhaPlantUpYPos + x, a);
    y = a;
    a = M(FlameYPosData + y);
    y = 0x00;
    compare(a, M(Enemy_Y_Position + x));
    if (!c)
        goto SetMF;
    ++y;

SetMF:
    a = M(FlameYMFAdderData + y);
    writeData(PiranhaPlantDownYPos + x, a);
    a = 0x00;
    writeData(EnemyFrenzyBuffer, a);

FinishFlame:
    a = 0x08;
    writeData(Enemy_BoundBoxCtrl + x, a);
    a = 0x01;
    writeData(Enemy_Y_HighPos + x, a);
    writeData(RedParatroopa + x, a);
    a >>= 1;
    writeData(RedPTroopaOrigXPos + x, a);
    writeData(Enemy_State + x, a);
    goto Return;
    // ;--------------------------------

InitFireworks:
    a = M(FrenzyEnemyTimer);
    if (!z)
        goto ExitFWk;
    a = 0x20;
    writeData(FrenzyEnemyTimer, a);
    --M(FireworksCounter);
    y = 0x06;

StarFChk:
    --y;
    a = M(Fireworks + y);
    compare(a, 0x31);
    if (!z)
        goto StarFChk;
    a = M(Enemy_X_Position + y);
    c = 1;
    a -= 0x30;
    pha();
    a = M(Enemy_PageLoc + y);
    a -= 0x00;
    writeData(TitleScreenModeValue, a);
    a = M(FireworksCounter);
    c = 0;
    a += M(Enemy_State + y);
    y = a;
    pla();
    c = 0;
    a += M(FireworksXPosData + y);
    writeData(Enemy_X_Position + x, a);
    a = M(TitleScreenModeValue);
    a += 0x00;
    writeData(Enemy_PageLoc + x, a);
    a = M(FireworksYPosData + y);
    writeData(Enemy_Y_Position + x, a);
    a = 0x01;
    writeData(Enemy_Y_HighPos + x, a);
    writeData(RedParatroopa + x, a);
    a >>= 1;
    writeData(ExplosionGfxCounter + x, a);
    a = 0x08;
    writeData(ExplosionTimerCounter + x, a);

ExitFWk:
    goto Return;
    // ;--------------------------------

BulletBillCheepCheep:
    a = M(FrenzyEnemyTimer);
    if (!z)
        goto ExF17;
    a = M(AreaType);
    if (!z)
        goto DoBulletBills;
    compare(x, 0x03);
    if (c)
        goto ExF17;
    y = 0x00;
    a = M(PseudoRandomBitReg + x);
    compare(a, 0xaa);
    if (!c)
        goto ChkW2;
    ++y;

ChkW2:
    a = M(WorldNumber);
    compare(a, 0x01);
    if (z)
        goto Get17ID;
    ++y;

Get17ID:
    a = y;
    a &= 0x01;
    y = a;
    a = M(SwimCC_IDData + y);

Set17ID:
    writeData(Fireworks + x, a);
    a = M(BitMFilter);
    compare(a, 0xff);
    if (!z)
        goto GetRBit;
    a = 0x00;
    writeData(BitMFilter, a);

GetRBit:
    a = M(PseudoRandomBitReg + x);
    a &= 0x07;

ChkRBit:
    y = a;
    a = M(Bitmasks + y);
    bit(M(BitMFilter));
    if (z)
        goto AddFBit;
    ++y;
    a = y;
    a &= 0x07;
    goto ChkRBit;

AddFBit:
    a |= M(BitMFilter);
    writeData(BitMFilter, a);
    a = M(Enemy17YPosData + y);
    JSR(PutAtRightExtent, 286);
    writeData(PiranhaPlantUpYPos + x, a);
    a = 0x20;
    writeData(FrenzyEnemyTimer, a);
    goto CheckpointEnemyID;

DoBulletBills:
    y = 0xff;

BB_SLoop:
    ++y;
    compare(y, 0x05);
    if (c)
        goto FireBulletBill;
    a = M(RedParatroopa + y);
    if (z)
        goto BB_SLoop;
    a = M(Fireworks + y);
    compare(a, 0x08);
    if (!z)
        goto BB_SLoop;

ExF17:
    goto Return;

FireBulletBill:
    a = M(Square2SoundQueue);
    a |= 0x08;
    writeData(Square2SoundQueue, a);
    a = 0x08;
    if (!z)
        goto Set17ID;
    // ;--------------------------------
    // ;$00 - used to store Y position of group enemies
    // ;$01 - used to store enemy ID
    // ;$02 - used to store page location of right side of screen
    // ;$03 - used to store X position of right side of screen

HandleGroupEnemies:
    y = 0x00;
    c = 1;
    a -= 0x37;
    pha();
    compare(a, 0x04);
    if (c)
        goto SnglID;
    pha();
    y = 0x06;
    a = M(PrimaryHardMode);
    if (z)
        goto PullID;
    y = 0x02;

PullID:
    pla();

SnglID:
    writeData(GameModeValue, y);
    y = 0xb0;
    a &= 0x02;
    if (z)
        goto SetYGp;
    y = 0x70;

SetYGp:
    writeData(TitleScreenModeValue, y);
    a = M(ScreenRight_PageLoc);
    writeData(VictoryModeValue, a);
    a = M(ScreenRight_X_Pos);
    writeData(GameOverModeValue, a);
    y = 0x02;
    pla();
    a >>= 1;
    if (!c)
        goto CntGrp;
    ++y;

CntGrp:
    writeData(NumberofGroupEnemies, y);

GrLoop:
    x = 0xff;

GSltLp:
    ++x;
    compare(x, 0x05);
    if (c)
        goto NextED;
    a = M(RedParatroopa + x);
    if (!z)
        goto GSltLp;
    a = M(GameModeValue);
    writeData(Fireworks + x, a);
    a = M(VictoryModeValue);
    writeData(Enemy_PageLoc + x, a);
    a = M(GameOverModeValue);
    writeData(Enemy_X_Position + x, a);
    c = 0;
    a += 0x18;
    writeData(GameOverModeValue, a);
    a = M(VictoryModeValue);
    a += 0x00;
    writeData(VictoryModeValue, a);
    a = M(TitleScreenModeValue);
    writeData(Enemy_Y_Position + x, a);
    a = 0x01;
    writeData(Enemy_Y_HighPos + x, a);
    writeData(RedParatroopa + x, a);
    JSR(CheckpointEnemyID, 287);
    --M(NumberofGroupEnemies);
    if (!z)
        goto GrLoop;

NextED:
    goto Inc2B;
    // ;--------------------------------

InitPiranhaPlant:
    a = 0x01;
    writeData(ExplosionGfxCounter + x, a);
    a >>= 1;
    writeData(Enemy_State + x, a);
    writeData(ExplosionTimerCounter + x, a);
    a = M(Enemy_Y_Position + x);
    writeData(PiranhaPlantDownYPos + x, a);
    c = 1;
    a -= 0x18;
    writeData(PiranhaPlantUpYPos + x, a);
    a = 0x09;
    goto SetBBox2;
    // ;--------------------------------

InitEnemyFrenzy:
    a = M(Fireworks + x);
    writeData(EnemyFrenzyBuffer, a);
    c = 1;
    a -= 0x12;
    switch (a) {
        case 0: goto LakituAndSpinyHandler;
        case 1: goto NoFrenzyCode;
        case 2: goto InitFlyingCheepCheep;
        case 3: goto InitBowserFlame;
        case 4: goto InitFireworks;
        case 5: goto BulletBillCheepCheep;
    }

NoFrenzyCode:
    goto Return;
    // ;--------------------------------

EndFrenzy:
    y = 0x05;

LakituChk:
    a = M(Fireworks + y);
    compare(a, 0x11);
    if (!z)
        goto NextFSlot;
    a = 0x01;
    writeData(Enemy_State + y, a);

NextFSlot:
    --y;
    if (!n)
        goto LakituChk;
    a = 0x00;
    writeData(EnemyFrenzyBuffer, a);
    writeData(RedParatroopa + x, a);
    goto Return;
    // ;--------------------------------

InitJumpGPTroopa:
    a = 0x02;
    writeData(Enemy_MovingDir + x, a);
    a = 0xf8;
    writeData(ExplosionGfxCounter + x, a);

TallBBox2:
    a = 0x03;

SetBBox2:
    writeData(Enemy_BoundBoxCtrl + x, a);
    goto Return;
    // ;--------------------------------

InitBalPlatform:
    --M(Enemy_Y_Position + x);
    --M(Enemy_Y_Position + x);
    y = M(SecondaryHardMode);
    if (!z)
        goto AlignP;
    y = 0x02;
    JSR(PosPlatform, 288);

AlignP:
    y = 0xff;
    a = M(BalPlatformAlignment);
    writeData(Enemy_State + x, a);
    if (!n)
        goto SetBPA;
    a = x;
    y = a;

SetBPA:
    writeData(BalPlatformAlignment, y);
    a = 0x00;
    writeData(Enemy_MovingDir + x, a);
    y = a;
    JSR(PosPlatform, 289);
    // ;--------------------------------

InitDropPlatform:
    a = 0xff;
    writeData(HammerThrowingTimer + x, a);
    goto CommonPlatCode;
    // ;--------------------------------

InitHoriPlatform:
    a = 0x00;
    writeData(ExplosionGfxCounter + x, a);
    goto CommonPlatCode;
    // ;--------------------------------

InitVertPlatform:
    y = 0x40;
    a = M(Enemy_Y_Position + x);
    if (!n)
        goto SetYO;
    a ^= 0xff;
    c = 0;
    a += 0x01;
    y = 0xc0;

SetYO:
    writeData(RedPTroopaOrigXPos + x, a);
    a = y;
    c = 0;
    a += M(Enemy_Y_Position + x);
    writeData(ExplosionGfxCounter + x, a);
    // ;--------------------------------

CommonPlatCode:
    JSR(InitVStf, 290);

SPBBox:
    a = 0x05;
    y = M(AreaType);
    compare(y, 0x03);
    if (z)
        goto CasPBB;
    y = M(SecondaryHardMode);
    if (!z)
        goto CasPBB;
    a = 0x06;

CasPBB:
    writeData(Enemy_BoundBoxCtrl + x, a);
    goto Return;
    // ;--------------------------------

LargeLiftUp:
    JSR(PlatLiftUp, 291);
    goto LargeLiftBBox;

LargeLiftDown:
    JSR(PlatLiftDown, 292);

LargeLiftBBox:
    goto SPBBox;
    // ;--------------------------------

PlatLiftUp:
    a = 0x10;
    writeData(PiranhaPlantDownYPos + x, a);
    a = 0xff;
    writeData(ExplosionTimerCounter + x, a);
    goto CommonSmallLift;
    // ;--------------------------------

PlatLiftDown:
    a = 0xf0;
    writeData(PiranhaPlantDownYPos + x, a);
    a = 0x00;
    writeData(ExplosionTimerCounter + x, a);
    // ;--------------------------------

CommonSmallLift:
    y = 0x01;
    JSR(PosPlatform, 293);
    a = 0x04;
    writeData(Enemy_BoundBoxCtrl + x, a);
    goto Return;
    // ;--------------------------------

PosPlatform:
    a = M(Enemy_X_Position + x);
    c = 0;
    a += M(PlatPosDataLow + y);
    writeData(Enemy_X_Position + x, a);
    a = M(Enemy_PageLoc + x);
    a += M(PlatPosDataHigh + y);
    writeData(Enemy_PageLoc + x, a);
    goto Return;
    // ;--------------------------------

EndOfEnemyInitCode:
    goto Return;
    // ;-------------------------------------------------------------------------------------

RunEnemyObjectsCore:
    x = M(BulletBill_FrenzyVar);
    a = 0x00;
    y = M(Fireworks + x);
    compare(y, 0x15);
    if (!c)
        goto loc_C88F;
    a = y;
    a -= 0x14;
loc_C88F:
    switch (a) {
        case 0: goto RunNormalEnemies;
        case 1: goto RunBowserFlame;
        case 2: goto RunFireworks;
        case 3: goto NoRunCode;
        case 4: goto NoRunCode;
        case 5: goto NoRunCode;
        case 6: goto NoRunCode;
        case 7: goto RunFirebarObj;
        case 8: goto RunFirebarObj;
        case 9: goto RunFirebarObj;
        case 10: goto RunFirebarObj;
        case 11: goto RunFirebarObj;
        case 12: goto RunFirebarObj;
        case 13: goto RunFirebarObj;
        case 14: goto RunFirebarObj;
        case 15: goto NoRunCode;
        case 16: goto RunLargePlatform;
        case 17: goto RunLargePlatform;
        case 18: goto RunLargePlatform;
        case 19: goto RunLargePlatform;
        case 20: goto RunLargePlatform;
        case 21: goto RunLargePlatform;
        case 22: goto RunLargePlatform;
        case 23: goto RunSmallPlatform;
        case 24: goto RunSmallPlatform;
        case 25: goto RunBowser;
        case 26: goto PowerUpObjHandler;
        case 27: goto VineObjectHandler;
        case 28: goto NoRunCode;
        case 29: goto RunStarFlagObj;
        case 30: goto JumpspringHandler;
        case 31: goto NoRunCode;
        case 32: goto WarpZoneObject;
        case 33: goto RunRetainerObj;
    }

NoRunCode:
    goto Return;
    // ;--------------------------------

RunRetainerObj:
    JSR(GetEnemyOffscreenBits, 294);
    JSR(RelativeEnemyPosition, 295);
    goto EnemyGfxHandler;
    // ;--------------------------------

RunNormalEnemies:
    a = 0x00;
    writeData(Enemy_SprAttrib + x, a);
    JSR(GetEnemyOffscreenBits, 296);
    JSR(RelativeEnemyPosition, 297);
    JSR(EnemyGfxHandler, 298);
    JSR(GetEnemyBoundBox, 299);
    JSR(EnemyToBGCollisionDet, 300);
    JSR(EnemiesCollision, 301);
    JSR(PlayerEnemyCollision, 302);
    y = M(TimerControl);
    if (!z)
        goto SkipMove;
    JSR(EnemyMovementSubs, 303);

SkipMove:
    goto OffscreenBoundsCheck;

EnemyMovementSubs:
    a = M(Fireworks + x);
    switch (a) {
        case 0: goto MoveNormalEnemy;
        case 1: goto MoveNormalEnemy;
        case 2: goto MoveNormalEnemy;
        case 3: goto MoveNormalEnemy;
        case 4: goto MoveNormalEnemy;
        case 5: goto ProcHammerBro;
        case 6: goto MoveNormalEnemy;
        case 7: goto MoveBloober;
        case 8: goto MoveBulletBill;
        case 9: goto NoMoveCode;
        case 10: goto MoveSwimmingCheepCheep;
        case 11: goto MoveSwimmingCheepCheep;
        case 12: goto MovePodoboo;
        case 13: goto MovePiranhaPlant;
        case 14: goto MoveJumpingEnemy;
        case 15: goto ProcMoveRedPTroopa;
        case 16: goto MoveFlyGreenPTroopa;
        case 17: goto MoveLakitu;
        case 18: goto MoveNormalEnemy;
        case 19: goto NoMoveCode;
        case 20: goto MoveFlyingCheepCheep;
    }

NoMoveCode:
    goto Return;
    // ;--------------------------------

RunBowserFlame:
    JSR(ProcBowserFlame, 304);
    JSR(GetEnemyOffscreenBits, 305);
    JSR(RelativeEnemyPosition, 306);
    JSR(GetEnemyBoundBox, 307);
    JSR(PlayerEnemyCollision, 308);
    goto OffscreenBoundsCheck;
    // ;--------------------------------

RunFirebarObj:
    JSR(ProcFirebar, 309);
    goto OffscreenBoundsCheck;
    // ;--------------------------------

RunSmallPlatform:
    JSR(GetEnemyOffscreenBits, 310);
    JSR(RelativeEnemyPosition, 311);
    JSR(SmallPlatformBoundBox, 312);
    JSR(SmallPlatformCollision, 313);
    JSR(RelativeEnemyPosition, 314);
    JSR(DrawSmallPlatform, 315);
    JSR(MoveSmallPlatform, 316);
    goto OffscreenBoundsCheck;
    // ;--------------------------------

RunLargePlatform:
    JSR(GetEnemyOffscreenBits, 317);
    JSR(RelativeEnemyPosition, 318);
    JSR(LargePlatformBoundBox, 319);
    JSR(LargePlatformCollision, 320);
    a = M(TimerControl);
    if (!z)
        goto SkipPT;
    JSR(LargePlatformSubroutines, 321);

SkipPT:
    JSR(RelativeEnemyPosition, 322);
    JSR(DrawLargePlatform, 323);
    goto OffscreenBoundsCheck;
    // ;--------------------------------

LargePlatformSubroutines:
    a = M(Fireworks + x);
    c = 1;
    a -= 0x24;
    switch (a) {
        case 0: goto BalancePlatform;
        case 1: goto YMovingPlatform;
        case 2: goto MoveLargeLiftPlat;
        case 3: goto MoveLargeLiftPlat;
        case 4: goto XMovingPlatform;
        case 5: goto DropPlatform;
        case 6: goto RightPlatform;
    }

EraseEnemyObject:
    a = 0x00;
    writeData(RedParatroopa + x, a);
    writeData(Fireworks + x, a);
    writeData(Enemy_State + x, a);
    writeData(FloateyNum_Control + x, a);
    writeData(EnemyIntervalTimer + x, a);
    writeData(ShellChainCounter + x, a);
    writeData(Enemy_SprAttrib + x, a);
    writeData(EnemyFrameTimer + x, a);
    goto Return;
    // ;-------------------------------------------------------------------------------------

MovePodoboo:
    a = M(EnemyIntervalTimer + x);
    if (!z)
        goto PdbM;
    JSR(InitPodoboo, 324);
    a = M(0x07a8 + x);
    a |= 0x80;
    writeData(PiranhaPlantDownYPos + x, a);
    a &= 0x0f;
    a |= 0x06;
    writeData(EnemyIntervalTimer + x, a);
    a = 0xf9;
    writeData(ExplosionTimerCounter + x, a);

PdbM:
    goto MoveJ_EnemyVertically;
    // ;--------------------------------
    // ;$00 - used in HammerBroJumpCode as bitmask

ProcHammerBro:
    a = M(Enemy_State + x);
    a &= 0x20;
    if (z)
        goto ChkJH;
    goto MoveDefeatedEnemy;

ChkJH:
    a = M(HammerBroJumpTimer + x);
    if (z)
        goto HammerBroJumpCode;
    --M(HammerBroJumpTimer + x);
    a = M(Enemy_OffscreenBits);
    a &= 0x0c;
    if (!z)
        goto MoveHammerBroXDir;
    a = M(HammerThrowingTimer + x);
    if (!z)
        goto DecHT;
    y = M(SecondaryHardMode);
    a = M(HammerThrowTmrData + y);
    writeData(HammerThrowingTimer + x, a);
    JSR(SpawnHammerObj, 325);
    if (!c)
        goto DecHT;
    a = M(Enemy_State + x);
    a |= 0x08;
    writeData(Enemy_State + x, a);
    goto MoveHammerBroXDir;

DecHT:
    --M(HammerThrowingTimer + x);
    goto MoveHammerBroXDir;

HammerBroJumpCode:
    a = M(Enemy_State + x);
    a &= 0x07;
    compare(a, 0x01);
    if (z)
        goto MoveHammerBroXDir;
    a = 0x00;
    writeData(TitleScreenModeValue, a);
    y = 0xfa;
    a = M(Enemy_Y_Position + x);
    if (n)
        goto SetHJ;
    y = 0xfd;
    compare(a, 0x70);
    ++M(TitleScreenModeValue);
    if (!c)
        goto SetHJ;
    --M(TitleScreenModeValue);
    a = M(0x07a8 + x);
    a &= 0x01;
    if (!z)
        goto SetHJ;
    y = 0xfa;

SetHJ:
    writeData(ExplosionTimerCounter + x, y);
    a = M(Enemy_State + x);
    a |= 0x01;
    writeData(Enemy_State + x, a);
    a = M(TitleScreenModeValue);
    a &= M(0x07a9 + x);
    y = a;
    a = M(SecondaryHardMode);
    if (!z)
        goto HJump;
    y = a;

HJump:
    a = M(HammerBroJumpLData + y);
    writeData(EnemyFrameTimer + x, a);
    a = M(0x07a8 + x);
    a |= 0xc0;
    writeData(HammerBroJumpTimer + x, a);

MoveHammerBroXDir:
    y = 0xfc;
    a = M(TallEnemy);
    a &= 0x40;
    if (!z)
        goto Shimmy;
    y = 0x04;

Shimmy:
    writeData(ExplosionGfxCounter + x, y);
    y = 0x01;
    JSR(PlayerEnemyDiff, 326);
    if (n)
        goto SetShim;
    ++y;
    a = M(EnemyIntervalTimer + x);
    if (!z)
        goto SetShim;
    a = 0xf8;
    writeData(ExplosionGfxCounter + x, a);

SetShim:
    writeData(Enemy_MovingDir + x, y);

MoveNormalEnemy:
    y = 0x00;
    a = M(Enemy_State + x);
    a &= 0x40;
    if (!z)
        goto FallE;
    a = M(Enemy_State + x);
    a <<= 1;
    if (c)
        goto SteadM;
    a = M(Enemy_State + x);
    a &= 0x20;
    if (!z)
        goto MoveDefeatedEnemy;
    a = M(Enemy_State + x);
    a &= 0x07;
    if (z)
        goto SteadM;
    compare(a, 0x05);
    if (z)
        goto FallE;
    compare(a, 0x03);
    if (c)
        goto ReviveStunned;

FallE:
    JSR(MoveD_EnemyVertically, 327);
    y = 0x00;
    a = M(Enemy_State + x);
    compare(a, 0x02);
    if (z)
        goto MEHor;
    a &= 0x40;
    if (z)
        goto SteadM;
    a = M(Fireworks + x);
    compare(a, 0x2e);
    if (z)
        goto SteadM;
    if (!z)
        goto SlowM;

MEHor:
    goto MoveEnemyHorizontally;

SlowM:
    y = 0x01;

SteadM:
    a = M(ExplosionGfxCounter + x);
    pha();
    if (!n)
        goto AddHS;
    ++y;
    ++y;

AddHS:
    c = 0;
    a += M(XSpeedAdderData + y);
    writeData(ExplosionGfxCounter + x, a);
    JSR(MoveEnemyHorizontally, 328);
    pla();
    writeData(ExplosionGfxCounter + x, a);
    goto Return;

ReviveStunned:
    a = M(EnemyIntervalTimer + x);
    if (!z)
        goto ChkKillGoomba;
    writeData(Enemy_State + x, a);
    a = M(TallEnemy);
    a &= 0x01;
    y = a;
    ++y;
    writeData(Enemy_MovingDir + x, y);
    --y;
    a = M(PrimaryHardMode);
    if (z)
        goto SetRSpd;
    ++y;
    ++y;

SetRSpd:
    a = M(RevivedXSpeed + y);
    writeData(ExplosionGfxCounter + x, a);
    goto Return;

MoveDefeatedEnemy:
    JSR(MoveD_EnemyVertically, 329);
    goto MoveEnemyHorizontally;

ChkKillGoomba:
    compare(a, 0x0e);
    if (!z)
        goto NKGmba;
    a = M(Fireworks + x);
    compare(a, 0x06);
    if (!z)
        goto NKGmba;
    JSR(EraseEnemyObject, 330);

NKGmba:
    goto Return;
    // ;--------------------------------

MoveJumpingEnemy:
    JSR(MoveJ_EnemyVertically, 331);
    goto MoveEnemyHorizontally;
    // ;--------------------------------

ProcMoveRedPTroopa:
    a = M(ExplosionTimerCounter + x);
    a |= M(PiranhaPlantDownYPos + x);
    if (!z)
        goto MoveRedPTUpOrDown;
    writeData(PiranhaPlantUpYPos + x, a);
    a = M(Enemy_Y_Position + x);
    compare(a, M(RedPTroopaOrigXPos + x));
    if (c)
        goto MoveRedPTUpOrDown;
    a = M(TallEnemy);
    a &= 0x07;
    if (!z)
        goto NoIncPT;
    ++M(Enemy_Y_Position + x);

NoIncPT:
    goto Return;

MoveRedPTUpOrDown:
    a = M(Enemy_Y_Position + x);
    compare(a, M(ExplosionGfxCounter + x));
    if (!c)
        goto MovPTDwn;
    goto MoveRedPTroopaUp;

MovPTDwn:
    goto MoveRedPTroopaDown;
    // ;--------------------------------
    // ;$00 - used to store adder for movement, also used as adder for platform
    // ;$01 - used to store maximum value for secondary counter

MoveFlyGreenPTroopa:
    JSR(XMoveCntr_GreenPTroopa, 332);
    JSR(MoveWithXMCntrs, 333);
    y = 0x01;
    a = M(TallEnemy);
    a &= 0x03;
    if (!z)
        goto NoMGPT;
    a = M(TallEnemy);
    a &= 0x40;
    if (!z)
        goto YSway;
    y = 0xff;

YSway:
    writeData(TitleScreenModeValue, y);
    a = M(Enemy_Y_Position + x);
    c = 0;
    a += M(TitleScreenModeValue);
    writeData(Enemy_Y_Position + x, a);

NoMGPT:
    goto Return;

XMoveCntr_GreenPTroopa:
    a = 0x13;

XMoveCntr_Platform:
    writeData(GameModeValue, a);
    a = M(TallEnemy);
    a &= 0x03;
    if (!z)
        goto NoIncXM;
    y = M(ExplosionGfxCounter + x);
    a = M(ExplosionTimerCounter + x);
    a >>= 1;
    if (c)
        goto DecSeXM;
    compare(y, M(GameModeValue));
    if (z)
        goto IncPXM;
    ++M(ExplosionGfxCounter + x);

NoIncXM:
    goto Return;

IncPXM:
    ++M(ExplosionTimerCounter + x);
    goto Return;

DecSeXM:
    a = y;
    if (z)
        goto IncPXM;
    --M(ExplosionGfxCounter + x);
    goto Return;

MoveWithXMCntrs:
    a = M(ExplosionGfxCounter + x);
    pha();
    y = 0x01;
    a = M(ExplosionTimerCounter + x);
    a &= 0x02;
    if (!z)
        goto XMRight;
    a = M(ExplosionGfxCounter + x);
    a ^= 0xff;
    c = 0;
    a += 0x01;
    writeData(ExplosionGfxCounter + x, a);
    y = 0x02;

XMRight:
    writeData(Enemy_MovingDir + x, y);
    JSR(MoveEnemyHorizontally, 334);
    writeData(TitleScreenModeValue, a);
    pla();
    writeData(ExplosionGfxCounter + x, a);
    goto Return;
    // ;--------------------------------

MoveBloober:
    a = M(Enemy_State + x);
    a &= 0x20;
    if (!z)
        goto MoveDefeatedBloober;
    y = M(SecondaryHardMode);
    a = M(0x07a8 + x);
    a &= M(BlooberBitmasks + y);
    if (!z)
        goto BlooberSwim;
    a = x;
    a >>= 1;
    if (!c)
        goto FBLeft;
    y = M(Player_MovingDir);
    if (c)
        goto SBMDir;

FBLeft:
    y = 0x02;
    JSR(PlayerEnemyDiff, 335);
    if (!n)
        goto SBMDir;
    --y;

SBMDir:
    writeData(Enemy_MovingDir + x, y);

BlooberSwim:
    JSR(ProcSwimmingB, 336);
    a = M(Enemy_Y_Position + x);
    c = 1;
    a -= M(PiranhaPlantDownYPos + x);
    compare(a, 0x20);
    if (!c)
        goto SwimX;
    writeData(Enemy_Y_Position + x, a);

SwimX:
    y = M(Enemy_MovingDir + x);
    --y;
    if (!z)
        goto LeftSwim;
    a = M(Enemy_X_Position + x);
    c = 0;
    a += M(ExplosionGfxCounter + x);
    writeData(Enemy_X_Position + x, a);
    a = M(Enemy_PageLoc + x);
    a += 0x00;
    writeData(Enemy_PageLoc + x, a);
    goto Return;

LeftSwim:
    a = M(Enemy_X_Position + x);
    c = 1;
    a -= M(ExplosionGfxCounter + x);
    writeData(Enemy_X_Position + x, a);
    a = M(Enemy_PageLoc + x);
    a -= 0x00;
    writeData(Enemy_PageLoc + x, a);
    goto Return;

MoveDefeatedBloober:
    goto MoveEnemySlowVert;

ProcSwimmingB:
    a = M(ExplosionTimerCounter + x);
    a &= 0x02;
    if (!z)
        goto ChkForFloatdown;
    a = M(TallEnemy);
    a &= 0x07;
    pha();
    a = M(ExplosionTimerCounter + x);
    a >>= 1;
    if (c)
        goto SlowSwim;
    pla();
    if (!z)
        goto BSwimE;
    a = M(PiranhaPlantDownYPos + x);
    c = 0;
    a += 0x01;
    writeData(PiranhaPlantDownYPos + x, a);
    writeData(ExplosionGfxCounter + x, a);
    compare(a, 0x02);
    if (!z)
        goto BSwimE;
    ++M(ExplosionTimerCounter + x);

BSwimE:
    goto Return;

SlowSwim:
    pla();
    if (!z)
        goto NoSSw;
    a = M(PiranhaPlantDownYPos + x);
    c = 1;
    a -= 0x01;
    writeData(PiranhaPlantDownYPos + x, a);
    writeData(ExplosionGfxCounter + x, a);
    if (!z)
        goto NoSSw;
    ++M(ExplosionTimerCounter + x);
    a = 0x02;
    writeData(EnemyIntervalTimer + x, a);

NoSSw:
    goto Return;

ChkForFloatdown:
    a = M(EnemyIntervalTimer + x);
    if (z)
        goto ChkNearPlayer;

Floatdown:
    a = M(TallEnemy);
    a >>= 1;
    if (c)
        goto NoFD;
    ++M(Enemy_Y_Position + x);

NoFD:
    goto Return;

ChkNearPlayer:
    a = M(Enemy_Y_Position + x);
    a += 0x10;
    compare(a, M(Player_Y_Position));
    if (!c)
        goto Floatdown;
    a = 0x00;
    writeData(ExplosionTimerCounter + x, a);
    goto Return;
    // ;--------------------------------

MoveBulletBill:
    a = M(Enemy_State + x);
    a &= 0x20;
    if (z)
        goto NotDefB;
    goto MoveJ_EnemyVertically;

NotDefB:
    a = 0xe8;
    writeData(ExplosionGfxCounter + x, a);
    goto MoveEnemyHorizontally;
    // ;--------------------------------
    // ;$02 - used to hold preset values
    // ;$03 - used to hold enemy state

MoveSwimmingCheepCheep:
    a = M(Enemy_State + x);
    a &= 0x20;
    if (z)
        goto CCSwim;
    goto MoveEnemySlowVert;

CCSwim:
    writeData(GameOverModeValue, a);
    a = M(Fireworks + x);
    c = 1;
    a -= 0x0a;
    y = a;
    a = M(SwimCCXMoveData + y);
    writeData(VictoryModeValue, a);
    a = M(RedPTroopaOrigXPos + x);
    c = 1;
    a -= M(VictoryModeValue);
    writeData(RedPTroopaOrigXPos + x, a);
    a = M(Enemy_X_Position + x);
    a -= 0x00;
    writeData(Enemy_X_Position + x, a);
    a = M(Enemy_PageLoc + x);
    a -= 0x00;
    writeData(Enemy_PageLoc + x, a);
    a = 0x20;
    writeData(VictoryModeValue, a);
    compare(x, 0x02);
    if (!c)
        goto ExSwCC;
    a = M(ExplosionGfxCounter + x);
    compare(a, 0x10);
    if (!c)
        goto CCSwimUpwards;
    a = M(PiranhaPlantUpYPos + x);
    c = 0;
    a += M(VictoryModeValue);
    writeData(PiranhaPlantUpYPos + x, a);
    a = M(Enemy_Y_Position + x);
    a += M(GameOverModeValue);
    writeData(Enemy_Y_Position + x, a);
    a = M(Enemy_Y_HighPos + x);
    a += 0x00;
    goto ChkSwimYPos;

CCSwimUpwards:
    a = M(PiranhaPlantUpYPos + x);
    c = 1;
    a -= M(VictoryModeValue);
    writeData(PiranhaPlantUpYPos + x, a);
    a = M(Enemy_Y_Position + x);
    a -= M(GameOverModeValue);
    writeData(Enemy_Y_Position + x, a);
    a = M(Enemy_Y_HighPos + x);
    a -= 0x00;

ChkSwimYPos:
    writeData(Enemy_Y_HighPos + x, a);
    y = 0x00;
    a = M(Enemy_Y_Position + x);
    c = 1;
    a -= M(PiranhaPlantDownYPos + x);
    if (!n)
        goto YPDiff;
    y = 0x10;
    a ^= 0xff;
    c = 0;
    a += 0x01;

YPDiff:
    compare(a, 0x0f);
    if (!c)
        goto ExSwCC;
    a = y;
    writeData(ExplosionGfxCounter + x, a);

ExSwCC:
    goto Return;
    // ;--------------------------------
    // ;$00 - used as counter for firebar parts
    // ;$01 - used for oscillated high byte of spin state or to hold horizontal adder
    // ;$02 - used for oscillated high byte of spin state or to hold vertical adder
    // ;$03 - used for mirror data
    // ;$04 - used to store player's sprite 1 X coordinate
    // ;$05 - used to evaluate mirror data
    // ;$06 - used to store either screen X coordinate or sprite data offset
    // ;$07 - used to store screen Y coordinate
    // ;$ed - used to hold maximum length of firebar
    // ;$ef - used to hold high byte of spinstate
    // ;horizontal adder is at first byte + high byte of spinstate,
    // ;vertical adder is same + 8 bytes, two's compliment
    // ;if greater than $08 for proper oscillation

ProcFirebar:
    JSR(GetEnemyOffscreenBits, 337);
    a = M(Enemy_OffscreenBits);
    a &= 0x08;
    if (!z)
        goto SkipFBar;
    a = M(TimerControl);
    if (!z)
        goto SusFbar;
    a = M(FirebarSpinSpeed + x);
    JSR(FirebarSpin, 338);
    a &= 0x1f;
    writeData(ExplosionTimerCounter + x, a);

SusFbar:
    a = M(ExplosionTimerCounter + x);
    y = M(Fireworks + x);
    compare(y, 0x1f);
    if (!c)
        goto SetupGFB;
    compare(a, 0x08);
    if (z)
        goto SkpFSte;
    compare(a, 0x18);
    if (!z)
        goto SetupGFB;

SkpFSte:
    c = 0;
    a += 0x01;
    writeData(ExplosionTimerCounter + x, a);

SetupGFB:
    writeData(0x00ef, a);
    JSR(RelativeEnemyPosition, 339);
    JSR(GetFirebarPosition, 340);
    y = M(Enemy_SprDataOffset + x);
    a = M(Enemy_Rel_YPos);
    writeData(Sprite_Y_Position + y, a);
    writeData(World8, a);
    a = M(Enemy_Rel_XPos);
    writeData(Sprite_X_Position + y, a);
    writeData(World7, a);
    a = 0x01;
    writeData(TitleScreenModeValue, a);
    JSR(FirebarCollision, 341);
    y = 0x05;
    a = M(Fireworks + x);
    compare(a, 0x1f);
    if (!c)
        goto SetMFbar;
    y = 0x0b;

SetMFbar:
    writeData(0x00ed, y);
    a = 0x00;
    writeData(TitleScreenModeValue, a);

DrawFbar:
    a = M(0x00ef);
    JSR(GetFirebarPosition, 342);
    JSR(DrawFirebar_Collision, 343);
    a = M(TitleScreenModeValue);
    compare(a, 0x04);
    if (!z)
        goto NextFbar;
    y = M(DuplicateObj_Offset);
    a = M(Enemy_SprDataOffset + y);
    writeData(World7, a);

NextFbar:
    ++M(TitleScreenModeValue);
    a = M(TitleScreenModeValue);
    compare(a, M(0x00ed));
    if (!c)
        goto DrawFbar;

SkipFBar:
    goto Return;

DrawFirebar_Collision:
    a = M(GameOverModeValue);
    writeData(World6, a);
    y = M(World7);
    a = M(GameModeValue);
    M(World6) >>= 1;
    if (c)
        goto AddHA;
    a ^= 0xff;
    a += 0x01;

AddHA:
    c = 0;
    a += M(Enemy_Rel_XPos);
    writeData(Sprite_X_Position + y, a);
    writeData(World7, a);
    compare(a, M(Enemy_Rel_XPos));
    if (c)
        goto SubtR1;
    a = M(Enemy_Rel_XPos);
    c = 1;
    a -= M(World7);
    goto ChkFOfs;

SubtR1:
    c = 1;
    a -= M(Enemy_Rel_XPos);

ChkFOfs:
    compare(a, 0x59);
    if (!c)
        goto VAHandl;
    a = 0xf8;
    if (!z)
        goto SetVFbr;

VAHandl:
    a = M(Enemy_Rel_YPos);
    compare(a, 0xf8);
    if (z)
        goto SetVFbr;
    a = M(VictoryModeValue);
    M(World6) >>= 1;
    if (c)
        goto AddVA;
    a ^= 0xff;
    a += 0x01;

AddVA:
    c = 0;
    a += M(Enemy_Rel_YPos);

SetVFbr:
    writeData(Sprite_Y_Position + y, a);
    writeData(World8, a);

FirebarCollision:
    JSR(DrawFirebar, 344);
    a = y;
    pha();
    a = M(StarInvincibleTimer);
    a |= M(TimerControl);
    if (!z)
        goto NoColFB;
    writeData(World6, a);
    y = M(Player_Y_HighPos);
    --y;
    if (!z)
        goto NoColFB;
    y = M(Player_Y_Position);
    a = M(PlayerSize);
    if (!z)
        goto AdjSm;
    a = M(CrouchingFlag);
    if (z)
        goto BigJp;

AdjSm:
    ++M(World6);
    ++M(World6);
    a = y;
    c = 0;
    a += 0x18;
    y = a;

BigJp:
    a = y;

FBCLoop:
    c = 1;
    a -= M(World8);
    if (!n)
        goto ChkVFBD;
    a ^= 0xff;
    c = 0;
    a += 0x01;

ChkVFBD:
    compare(a, 0x08);
    if (c)
        goto Chk2Ofs;
    a = M(World7);
    compare(a, 0xf0);
    if (c)
        goto Chk2Ofs;
    a = M(0x0207);
    c = 0;
    a += 0x04;
    writeData(World5, a);
    c = 1;
    a -= M(World7);
    if (!n)
        goto ChkFBCl;
    a ^= 0xff;
    c = 0;
    a += 0x01;

ChkFBCl:
    compare(a, 0x08);
    if (!c)
        goto ChgSDir;

Chk2Ofs:
    a = M(World6);
    compare(a, 0x02);
    if (z)
        goto NoColFB;
    y = M(World6);
    a = M(Player_Y_Position);
    c = 0;
    a += M(FirebarYPos + y);
    ++M(World6);
    goto FBCLoop;

ChgSDir:
    x = 0x01;
    a = M(World5);
    compare(a, M(World7));
    if (c)
        goto SetSDir;
    ++x;

SetSDir:
    writeData(Enemy_MovingDir, x);
    x = 0x00;
    a = M(TitleScreenModeValue);
    pha();
    JSR(InjurePlayer, 345);
    pla();
    writeData(TitleScreenModeValue, a);

NoColFB:
    pla();
    c = 0;
    a += 0x04;
    writeData(World7, a);
    x = M(BulletBill_FrenzyVar);
    goto Return;

GetFirebarPosition:
    pha();
    a &= 0x0f;
    compare(a, 0x09);
    if (!c)
        goto GetHAdder;
    a ^= 0x0f;
    c = 0;
    a += 0x01;

GetHAdder:
    writeData(GameModeValue, a);
    y = M(TitleScreenModeValue);
    a = M(FirebarTblOffsets + y);
    c = 0;
    a += M(GameModeValue);
    y = a;
    a = M(FirebarPosLookupTbl + y);
    writeData(GameModeValue, a);
    pla();
    pha();
    c = 0;
    a += 0x08;
    a &= 0x0f;
    compare(a, 0x09);
    if (!c)
        goto GetVAdder;
    a ^= 0x0f;
    c = 0;
    a += 0x01;

GetVAdder:
    writeData(VictoryModeValue, a);
    y = M(TitleScreenModeValue);
    a = M(FirebarTblOffsets + y);
    c = 0;
    a += M(VictoryModeValue);
    y = a;
    a = M(FirebarPosLookupTbl + y);
    writeData(VictoryModeValue, a);
    pla();
    a >>= 1;
    a >>= 1;
    a >>= 1;
    y = a;
    a = M(FirebarMirrorData + y);
    writeData(GameOverModeValue, a);
    goto Return;
    // ;--------------------------------

MoveFlyingCheepCheep:
    a = M(Enemy_State + x);
    a &= 0x20;
    if (z)
        goto FlyCC;
    a = 0x00;
    writeData(Enemy_SprAttrib + x, a);
    goto MoveJ_EnemyVertically;

FlyCC:
    JSR(MoveEnemyHorizontally, 346);
    y = 0x0d;
    a = 0x05;
    JSR(SetXMoveAmt, 347);
    a = M(PiranhaPlantDownYPos + x);
    a >>= 1;
    a >>= 1;
    a >>= 1;
    a >>= 1;
    y = a;
    a = M(Enemy_Y_Position + x);
    c = 1;
    a -= M(PRandomSubtracter + y);
    if (!n)
        goto AddCCF;
    a ^= 0xff;
    c = 0;
    a += 0x01;

AddCCF:
    compare(a, 0x08);
    if (c)
        goto BPGet;
    a = M(PiranhaPlantDownYPos + x);
    c = 0;
    a += 0x10;
    writeData(PiranhaPlantDownYPos + x, a);
    a >>= 1;
    a >>= 1;
    a >>= 1;
    a >>= 1;
    y = a;

BPGet:
    a = M(FlyCCBPriority + y);
    writeData(Enemy_SprAttrib + x, a);
    goto Return;
    // ;--------------------------------
    // ;$00 - used to hold horizontal difference
    // ;$01-$03 - used to hold difference adjusters

MoveLakitu:
    a = M(Enemy_State + x);
    a &= 0x20;
    if (z)
        goto ChkLS;
    goto MoveD_EnemyVertically;

ChkLS:
    a = M(Enemy_State + x);
    if (z)
        goto Fr12S;
    a = 0x00;
    writeData(ExplosionTimerCounter + x, a);
    writeData(EnemyFrenzyBuffer, a);
    a = 0x10;
    if (!z)
        goto SetLSpd;

Fr12S:
    a = 0x12;
    writeData(EnemyFrenzyBuffer, a);
    y = 0x02;

LdLDa:
    a = M(LakituDiffAdj + y);
    writeData(GameModeValue + y, a);
    --y;
    if (!n)
        goto LdLDa;
    JSR(PlayerLakituDiff, 348);

SetLSpd:
    writeData(ExplosionGfxCounter + x, a);
    y = 0x01;
    a = M(ExplosionTimerCounter + x);
    a &= 0x01;
    if (!z)
        goto SetLMov;
    a = M(ExplosionGfxCounter + x);
    a ^= 0xff;
    c = 0;
    a += 0x01;
    writeData(ExplosionGfxCounter + x, a);
    ++y;

SetLMov:
    writeData(Enemy_MovingDir + x, y);
    goto MoveEnemyHorizontally;

PlayerLakituDiff:
    y = 0x00;
    JSR(PlayerEnemyDiff, 349);
    if (!n)
        goto ChkLakDif;
    ++y;
    a = M(TitleScreenModeValue);
    a ^= 0xff;
    c = 0;
    a += 0x01;
    writeData(TitleScreenModeValue, a);

ChkLakDif:
    a = M(TitleScreenModeValue);
    compare(a, 0x3c);
    if (!c)
        goto ChkPSpeed;
    a = 0x3c;
    writeData(TitleScreenModeValue, a);
    a = M(Fireworks + x);
    compare(a, 0x11);
    if (!z)
        goto ChkPSpeed;
    a = y;
    compare(a, M(ExplosionTimerCounter + x));
    if (z)
        goto ChkPSpeed;
    a = M(ExplosionTimerCounter + x);
    if (z)
        goto SetLMovD;
    --M(ExplosionGfxCounter + x);
    a = M(ExplosionGfxCounter + x);
    if (!z)
        goto ExMoveLak;

SetLMovD:
    a = y;
    writeData(ExplosionTimerCounter + x, a);

ChkPSpeed:
    a = M(TitleScreenModeValue);
    a &= 0x3c;
    a >>= 1;
    a >>= 1;
    writeData(TitleScreenModeValue, a);
    y = 0x00;
    a = M(Player_X_Speed);
    if (z)
        goto SubDifAdj;
    a = M(ScrollAmount);
    if (z)
        goto SubDifAdj;
    ++y;
    a = M(Player_X_Speed);
loc_CFB5:
    compare(a, 0x19);
    if (!c)
        goto ChkSpinyO;
    a = M(ScrollAmount);
    compare(a, 0x02);
    if (!c)
        goto ChkSpinyO;
    ++y;

ChkSpinyO:
    a = M(Fireworks + x);
    compare(a, 0x12);
    if (!z)
        goto ChkEmySpd;
    a = M(Player_X_Speed);
    if (!z)
        goto SubDifAdj;

ChkEmySpd:
    a = M(ExplosionTimerCounter + x);
    if (!z)
        goto SubDifAdj;
    y = 0x00;

SubDifAdj:
    a = M(GameModeValue + y);
    y = M(TitleScreenModeValue);

SPixelLak:
    c = 1;
    a -= 0x01;
    --y;
    if (!n)
        goto SPixelLak;

ExMoveLak:
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$04-$05 - used to store name table address in little endian order

BridgeCollapse:
    x = M(BowserFront_Offset);
    a = M(Fireworks + x);
    compare(a, 0x2d);
    if (!z)
        goto SetM2;
    writeData(BulletBill_FrenzyVar, x);
    a = M(Enemy_State + x);
    if (z)
        goto RemoveBridge;
    a &= 0x40;
    if (z)
        goto SetM2;
    a = M(Enemy_Y_Position + x);
    compare(a, 0xe0);
    if (!c)
        goto MoveD_Bowser;

SetM2:
    a = 0x80;
    writeData(EventMusicQueue, a);
    ++M(OperMode_Task);
    goto KillAllEnemies;

MoveD_Bowser:
    JSR(MoveEnemySlowVert, 350);
    goto BowserGfxHandler;

RemoveBridge:
    --M(BowserFeetCounter);
    if (!z)
        goto NoBFall;
    a = 0x04;
    writeData(BowserFeetCounter, a);
    a = M(BowserBodyControls);
    a ^= 0x01;
    writeData(BowserBodyControls, a);
    a = 0x22;
    writeData(World6, a);
    y = M(BridgeCollapseOffset);
    a = M(BridgeCollapseData + y);
    writeData(World5, a);
    y = M(VRAM_Buffer1_Offset);
    ++y;
    x = 0x0c;
    JSR(RemBridge, 351);
    x = M(BulletBill_FrenzyVar);
    JSR(MoveVOffset, 352);
    a = 0x08;
    writeData(Square2SoundQueue, a);
    a = 0x01;
    writeData(NoiseSoundQueue, a);
    ++M(BridgeCollapseOffset);
    a = M(BridgeCollapseOffset);
    compare(a, 0x0f);
    if (!z)
        goto NoBFall;
    JSR(InitVStf, 353);
    a = 0x40;
    writeData(Enemy_State + x, a);
    a = 0x80;
    writeData(Square2SoundQueue, a);

NoBFall:
    goto BowserGfxHandler;
    // ;--------------------------------

RunBowser:
    a = M(Enemy_State + x);
    a &= 0x20;
    if (z)
        goto BowserControl;
    a = M(Enemy_Y_Position + x);
    compare(a, 0xe0);
    if (!c)
        goto MoveD_Bowser;

KillAllEnemies:
    x = 0x04;

KillLoop:
    JSR(EraseEnemyObject, 354);
    --x;
    if (!n)
        goto KillLoop;
    writeData(EnemyFrenzyBuffer, a);
    x = M(BulletBill_FrenzyVar);
    goto Return;

BowserControl:
    a = 0x00;
    writeData(EnemyFrenzyBuffer, a);
    a = M(TimerControl);
    if (z)
        goto ChkMouth;
    goto SkipToFB;

ChkMouth:
    a = M(BowserBodyControls);
    if (!n)
        goto FeetTmr;
    goto HammerChk;

FeetTmr:
    --M(BowserFeetCounter);
    if (!z)
        goto ResetMDr;
    a = 0x20;
    writeData(BowserFeetCounter, a);
    a = M(BowserBodyControls);
    a ^= 0x01;
    writeData(BowserBodyControls, a);

ResetMDr:
    a = M(TallEnemy);
    a &= 0x0f;
    if (!z)
        goto B_FaceP;
    a = 0x02;
    writeData(Enemy_MovingDir + x, a);

B_FaceP:
    a = M(EnemyFrameTimer + x);
    if (z)
        goto GetPRCmp;
    JSR(PlayerEnemyDiff, 355);
    if (!n)
        goto GetPRCmp;
    a = 0x01;
    writeData(Enemy_MovingDir + x, a);
    a = 0x02;
    writeData(BowserMovementSpeed, a);
    a = 0x20;
    writeData(EnemyFrameTimer + x, a);
    writeData(BowserFireBreathTimer, a);
    a = M(Enemy_X_Position + x);
    compare(a, 0xc8);
    if (c)
        goto HammerChk;

GetPRCmp:
    a = M(TallEnemy);
    a &= 0x03;
    if (!z)
        goto HammerChk;
    a = M(Enemy_X_Position + x);
    compare(a, M(BowserOrigXPos));
    if (!z)
        goto GetDToO;
    a = M(PseudoRandomBitReg + x);
    a &= 0x03;
    y = a;
    a = M(PRandomRange + y);
    writeData(MaxRangeFromOrigin, a);

GetDToO:
    a = M(Enemy_X_Position + x);
    c = 0;
    a += M(BowserMovementSpeed);
    writeData(Enemy_X_Position + x, a);
    y = M(Enemy_MovingDir + x);
    compare(y, 0x01);
    if (z)
        goto HammerChk;
    y = 0xff;
    c = 1;
    a -= M(BowserOrigXPos);
    if (!n)
        goto CompDToO;
    a ^= 0xff;
    c = 0;
    a += 0x01;
    y = 0x01;

CompDToO:
    compare(a, M(MaxRangeFromOrigin));
    if (!c)
        goto HammerChk;
    writeData(BowserMovementSpeed, y);

HammerChk:
    a = M(EnemyFrameTimer + x);
    if (!z)
        goto MakeBJump;
    JSR(MoveEnemySlowVert, 356);
    a = M(WorldNumber);
    compare(a, 0x05);
    if (!c)
        goto SetHmrTmr;
    a = M(TallEnemy);
    a &= 0x03;
    if (!z)
        goto SetHmrTmr;
    JSR(SpawnHammerObj, 357);

SetHmrTmr:
    a = M(Enemy_Y_Position + x);
    compare(a, 0x80);
    if (!c)
        goto ChkFireB;
    a = M(PseudoRandomBitReg + x);
    a &= 0x03;
    y = a;
    a = M(PRandomRange + y);
    writeData(EnemyFrameTimer + x, a);

SkipToFB:
    goto ChkFireB;

MakeBJump:
    compare(a, 0x01);
    if (!z)
        goto ChkFireB;
    --M(Enemy_Y_Position + x);
    JSR(InitVStf, 358);
    a = 0xfe;
    writeData(ExplosionTimerCounter + x, a);

ChkFireB:
    a = M(WorldNumber);
    compare(a, 0x07);
    if (z)
        goto SpawnFBr;
    compare(a, 0x05);
    if (c)
        goto BowserGfxHandler;

SpawnFBr:
    a = M(BowserFireBreathTimer);
    if (!z)
        goto BowserGfxHandler;
    a = 0x20;
    writeData(BowserFireBreathTimer, a);
    a = M(BowserBodyControls);
    a ^= 0x80;
    writeData(BowserBodyControls, a);
    if (n)
        goto ChkFireB;
    JSR(SetFlameTimer, 359);
    y = M(SecondaryHardMode);
    if (z)
        goto SetFBTmr;
    c = 1;
    a -= 0x10;

SetFBTmr:
    writeData(BowserFireBreathTimer, a);
    a = 0x15;
    writeData(EnemyFrenzyBuffer, a);
    // ;--------------------------------

BowserGfxHandler:
    JSR(ProcessBowserHalf, 360);
    y = 0x10;
    a = M(Enemy_MovingDir + x);
    a >>= 1;
    if (!c)
        goto CopyFToR;
    y = 0xf0;

CopyFToR:
    a = y;
    c = 0;
    a += M(Enemy_X_Position + x);
    y = M(DuplicateObj_Offset);
    writeData(Enemy_X_Position + y, a);
    a = M(Enemy_Y_Position + x);
    c = 0;
    a += 0x08;
    writeData(Enemy_Y_Position + y, a);
    a = M(Enemy_State + x);
    writeData(Enemy_State + y, a);
    a = M(Enemy_MovingDir + x);
    writeData(Enemy_MovingDir + y, a);
    a = M(BulletBill_FrenzyVar);
    pha();
    x = M(DuplicateObj_Offset);
    writeData(BulletBill_FrenzyVar, x);
    a = 0x2d;
    writeData(Fireworks + x, a);
    JSR(ProcessBowserHalf, 361);
    pla();
    writeData(BulletBill_FrenzyVar, a);
    x = a;
    a = 0x00;
    writeData(BowserGfxFlag, a);

ExBGfxH:
    goto Return;

ProcessBowserHalf:
    ++M(BowserGfxFlag);
    JSR(RunRetainerObj, 362);
    a = M(Enemy_State + x);
    if (!z)
        goto ExBGfxH;
    a = 0x0a;
    writeData(Enemy_BoundBoxCtrl + x, a);
    JSR(GetEnemyBoundBox, 363);
    goto PlayerEnemyCollision;
    // ;-------------------------------------------------------------------------------------
    // ;$00 - used to hold movement force and tile number
    // ;$01 - used to hold sprite attribute data

SetFlameTimer:
    y = M(BowserFlameTimerCtrl);
    ++M(BowserFlameTimerCtrl);
    a = M(BowserFlameTimerCtrl);
    a &= 0x07;
    writeData(BowserFlameTimerCtrl, a);
    a = M(FlameTimerData + y);

ExFl:
    goto Return;

ProcBowserFlame:
    a = M(TimerControl);
    if (!z)
        goto SetGfxF;
    a = 0x40;
    y = M(SecondaryHardMode);
    if (z)
        goto SFlmX;
    a = 0x60;

SFlmX:
    writeData(TitleScreenModeValue, a);
    a = M(RedPTroopaOrigXPos + x);
    c = 1;
    a -= M(TitleScreenModeValue);
    writeData(RedPTroopaOrigXPos + x, a);
    a = M(Enemy_X_Position + x);
    a -= 0x01;
    writeData(Enemy_X_Position + x, a);
    a = M(Enemy_PageLoc + x);
    a -= 0x00;
    writeData(Enemy_PageLoc + x, a);
    y = M(PiranhaPlantUpYPos + x);
    a = M(Enemy_Y_Position + x);
    compare(a, M(FlameYPosData + y));
    if (z)
        goto SetGfxF;
    c = 0;
    a += M(PiranhaPlantDownYPos + x);
    writeData(Enemy_Y_Position + x, a);

SetGfxF:
    JSR(RelativeEnemyPosition, 364);
    a = M(Enemy_State + x);
    if (!z)
        goto ExFl;
    a = 0x51;
    writeData(TitleScreenModeValue, a);
    y = 0x02;
    a = M(TallEnemy);
    a &= 0x02;
    if (z)
        goto FlmeAt;
    y = 0x82;

FlmeAt:
    writeData(GameModeValue, y);
    y = M(Enemy_SprDataOffset + x);
    x = 0x00;

DrawFlameLoop:
    a = M(Enemy_Rel_YPos);
    writeData(Sprite_Y_Position + y, a);
    a = M(TitleScreenModeValue);
    writeData(Sprite_Tilenumber + y, a);
    ++M(TitleScreenModeValue);
    a = M(GameModeValue);
    writeData(Sprite_Attributes + y, a);
    a = M(Enemy_Rel_XPos);
    writeData(Sprite_X_Position + y, a);
    c = 0;
    a += 0x08;
    writeData(Enemy_Rel_XPos, a);
    ++y;
    ++y;
    ++y;
    ++y;
    ++x;
    compare(x, 0x03);
    if (!c)
        goto DrawFlameLoop;
    x = M(BulletBill_FrenzyVar);
    JSR(GetEnemyOffscreenBits, 365);
    y = M(Enemy_SprDataOffset + x);
    a = M(Enemy_OffscreenBits);
    a >>= 1;
    pha();
    if (!c)
        goto M3FOfs;
    a = 0xf8;
    writeData(0x020c + y, a);

M3FOfs:
    pla();
    a >>= 1;
    pha();
    if (!c)
        goto M2FOfs;
    a = 0xf8;
    writeData(0x0208 + y, a);

M2FOfs:
    pla();
    a >>= 1;
    pha();
    if (!c)
        goto M1FOfs;
    a = 0xf8;
    writeData(0x0204 + y, a);

M1FOfs:
    pla();
    a >>= 1;
    if (!c)
        goto ExFlmeD;
    a = 0xf8;
    writeData(Sprite_Y_Position + y, a);

ExFlmeD:
    goto Return;
    // ;--------------------------------

RunFireworks:
    --M(ExplosionTimerCounter + x);
    if (!z)
        goto SetupExpl;
    a = 0x08;
    writeData(ExplosionTimerCounter + x, a);
    ++M(ExplosionGfxCounter + x);
    a = M(ExplosionGfxCounter + x);
    compare(a, 0x03);
    if (c)
        goto FireworksSoundScore;

SetupExpl:
    JSR(RelativeEnemyPosition, 366);
    a = M(Enemy_Rel_YPos);
    writeData(Fireball_Rel_YPos, a);
    a = M(Enemy_Rel_XPos);
    writeData(Fireball_Rel_XPos, a);
    y = M(Enemy_SprDataOffset + x);
    a = M(ExplosionGfxCounter + x);
    JSR(DrawExplosion_Fireworks, 367);
    goto Return;

FireworksSoundScore:
    a = 0x00;
    writeData(RedParatroopa + x, a);
    a = 0x08;
    writeData(Square2SoundQueue, a);
    a = 0x05;
    writeData(0x0138, a);
    goto EndAreaPoints;
    // ;--------------------------------

RunStarFlagObj:
    a = 0x00;
    writeData(EnemyFrenzyBuffer, a);
    a = M(StarFlagTaskControl);
    compare(a, 0x05);
    if (c)
        goto StarFlagExit;
    switch (a) {
        case 0: goto StarFlagExit;
        case 1: goto GameTimerFireworks;
        case 2: goto AwardGameTimerPoints;
        case 3: goto RaiseFlagSetoffFWorks;
        case 4: goto DelayToAreaEnd;
    }

GameTimerFireworks:
    y = 0x05;
    a = M(0x07fa);
    compare(a, 0x01);
    if (z)
        goto SetFWC;
    y = 0x03;
    compare(a, 0x03);
    if (z)
        goto SetFWC;
    y = 0x00;
    compare(a, 0x06);
    if (z)
        goto SetFWC;
    a = 0xff;

SetFWC:
    writeData(FireworksCounter, a);
    writeData(Enemy_State + x, y);

IncrementSFTask1:
    ++M(StarFlagTaskControl);

StarFlagExit:
    goto Return;

AwardGameTimerPoints:
    a = M(GameTimerDisplay);
    a |= M(0x07f9);
    a |= M(0x07fa);
    if (z)
        goto IncrementSFTask1;
    a = M(TallEnemy);
    a &= 0x04;
    if (z)
        goto NoTTick;
    a = 0x10;
    writeData(Square2SoundQueue, a);

NoTTick:
    y = 0x23;
    a = 0xff;
    writeData(0x0139, a);
    JSR(DigitsMathRoutine, 368);
    a = 0x05;
    writeData(0x0139, a);

EndAreaPoints:
    y = 0x0b;
    a = M(CurrentPlayer);
    if (z)
        goto ELPGive;
    y = 0x11;

ELPGive:
    JSR(DigitsMathRoutine, 369);
    a = M(CurrentPlayer);
    a <<= 1;
    a <<= 1;
    a <<= 1;
    a <<= 1;
    a |= 0x04;
    goto UpdateNumber;

RaiseFlagSetoffFWorks:
    a = M(Enemy_Y_Position + x);
    compare(a, 0x72);
    if (!c)
        goto SetoffF;
    --M(Enemy_Y_Position + x);
    goto DrawStarFlag;

SetoffF:
    a = M(FireworksCounter);
    if (z)
        goto DrawFlagSetTimer;
    if (n)
        goto DrawFlagSetTimer;
    a = 0x16;
    writeData(EnemyFrenzyBuffer, a);

DrawStarFlag:
    JSR(RelativeEnemyPosition, 370);
    y = M(Enemy_SprDataOffset + x);
    x = 0x03;

DSFLoop:
    a = M(Enemy_Rel_YPos);
    c = 0;
    a += M(StarFlagYPosAdder + x);
    writeData(Sprite_Y_Position + y, a);
    a = M(StarFlagTileData + x);
    writeData(Sprite_Tilenumber + y, a);
    a = 0x22;
    writeData(Sprite_Attributes + y, a);
    a = M(Enemy_Rel_XPos);
    c = 0;
    a += M(StarFlagXPosAdder + x);
    writeData(Sprite_X_Position + y, a);
    ++y;
    ++y;
    ++y;
    ++y;
    --x;
    if (!n)
        goto DSFLoop;
    x = M(BulletBill_FrenzyVar);
    goto Return;

DrawFlagSetTimer:
    JSR(DrawStarFlag, 371);
    a = 0x06;
    writeData(EnemyIntervalTimer + x, a);

IncrementSFTask2:
    ++M(StarFlagTaskControl);
    goto Return;

DelayToAreaEnd:
    JSR(DrawStarFlag, 372);
    a = M(EnemyIntervalTimer + x);
    if (!z)
        goto StarFlagExit2;
    a = M(EventMusicBuffer);
    if (z)
        goto IncrementSFTask2;

StarFlagExit2:
    goto Return;
    // ;--------------------------------
    // ;$00 - used to store horizontal difference between player and piranha plant

MovePiranhaPlant:
    a = M(Enemy_State + x);
    if (!z)
        goto PutinPipe;
    a = M(EnemyFrameTimer + x);
    if (!z)
        goto PutinPipe;
    a = M(ExplosionTimerCounter + x);
    if (!z)
        goto SetupToMovePPlant;
    a = M(ExplosionGfxCounter + x);
    if (n)
        goto ReversePlantSpeed;
    JSR(PlayerEnemyDiff, 373);
    if (!n)
        goto ChkPlayerNearPipe;
    a = M(TitleScreenModeValue);
    a ^= 0xff;
    c = 0;
    a += 0x01;
    writeData(TitleScreenModeValue, a);

ChkPlayerNearPipe:
    a = M(TitleScreenModeValue);
    compare(a, 0x21);
    if (!c)
        goto PutinPipe;

ReversePlantSpeed:
    a = M(ExplosionGfxCounter + x);
    a ^= 0xff;
    c = 0;
    a += 0x01;
    writeData(ExplosionGfxCounter + x, a);
    ++M(ExplosionTimerCounter + x);

SetupToMovePPlant:
    a = M(PiranhaPlantDownYPos + x);
    y = M(ExplosionGfxCounter + x);
    if (!n)
        goto RiseFallPiranhaPlant;
    a = M(PiranhaPlantUpYPos + x);

RiseFallPiranhaPlant:
    writeData(TitleScreenModeValue, a);
    a = M(TallEnemy);
    a >>= 1;
    if (!c)
        goto PutinPipe;
    a = M(TimerControl);
    if (!z)
        goto PutinPipe;
    a = M(Enemy_Y_Position + x);
    c = 0;
    a += M(ExplosionGfxCounter + x);
    writeData(Enemy_Y_Position + x, a);
    compare(a, M(TitleScreenModeValue));
    if (!z)
        goto PutinPipe;
    a = 0x00;
    writeData(ExplosionTimerCounter + x, a);
    a = 0x40;
    writeData(EnemyFrameTimer + x, a);

PutinPipe:
    a = 0x20;
    writeData(Enemy_SprAttrib + x, a);
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$07 - spinning speed

FirebarSpin:
    writeData(World8, a);
    a = M(FirebarSpinDirection + x);
    if (!z)
        goto SpinCounterClockwise;
    y = 0x18;
    a = M(ExplosionGfxCounter + x);
    c = 0;
    a += M(World8);
    writeData(ExplosionGfxCounter + x, a);
    a = M(ExplosionTimerCounter + x);
    a += 0x00;
    goto Return;

SpinCounterClockwise:
    y = 0x08;
    a = M(ExplosionGfxCounter + x);
    c = 1;
    a -= M(World8);
    writeData(ExplosionGfxCounter + x, a);
    a = M(ExplosionTimerCounter + x);
    a -= 0x00;
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$00 - used to hold collision flag, Y movement force + 5 or low byte of name table for rope
    // ;$01 - used to hold high byte of name table for rope
    // ;$02 - used to hold page location of rope

BalancePlatform:
    a = M(Enemy_Y_HighPos + x);
    compare(a, 0x03);
    if (!z)
        goto DoBPl;
    goto EraseEnemyObject;

DoBPl:
    a = M(Enemy_State + x);
    if (!n)
        goto CheckBalPlatform;
    goto Return;

CheckBalPlatform:
    y = a;
    a = M(HammerThrowingTimer + x);
    writeData(TitleScreenModeValue, a);
    a = M(Enemy_MovingDir + x);
    if (z)
        goto ChkForFall;
    goto PlatformFall;

ChkForFall:
    a = 0x2d;
    compare(a, M(Enemy_Y_Position + x));
    if (!c)
        goto ChkOtherForFall;
    compare(y, M(TitleScreenModeValue));
    if (z)
        goto MakePlatformFall;
    c = 0;
    a += 0x02;
    writeData(Enemy_Y_Position + x, a);
    goto StopPlatforms;

MakePlatformFall:
    goto InitPlatformFall;

ChkOtherForFall:
    compare(a, M(Enemy_Y_Position + y));
    if (!c)
        goto ChkToMoveBalPlat;
    compare(x, M(TitleScreenModeValue));
    if (z)
        goto MakePlatformFall;
    c = 0;
    a += 0x02;
    writeData(Enemy_Y_Position + y, a);
    goto StopPlatforms;

ChkToMoveBalPlat:
    a = M(Enemy_Y_Position + x);
    pha();
    a = M(HammerThrowingTimer + x);
    if (!n)
        goto ColFlg;
    a = M(PiranhaPlantDownYPos + x);
    c = 0;
    a += 0x05;
    writeData(TitleScreenModeValue, a);
    a = M(ExplosionTimerCounter + x);
    a += 0x00;
    if (n)
        goto PlatDn;
    if (!z)
        goto PlatUp;
    a = M(TitleScreenModeValue);
    compare(a, 0x0b);
    if (!c)
        goto PlatSt;
    if (c)
        goto PlatUp;

ColFlg:
    compare(a, M(BulletBill_FrenzyVar));
    if (z)
        goto PlatDn;

PlatUp:
    JSR(MovePlatformUp, 374);
    goto DoOtherPlatform;

PlatSt:
    JSR(StopPlatforms, 375);
    goto DoOtherPlatform;

PlatDn:
    JSR(MovePlatformDown, 376);

DoOtherPlatform:
    y = M(Enemy_State + x);
    pla();
    c = 1;
    a -= M(Enemy_Y_Position + x);
    c = 0;
    a += M(Enemy_Y_Position + y);
    writeData(Enemy_Y_Position + y, a);
    a = M(HammerThrowingTimer + x);
    if (n)
        goto DrawEraseRope;
    x = a;
    JSR(PositionPlayerOnVPlat, 377);

DrawEraseRope:
    y = M(BulletBill_FrenzyVar);
    a = M(ExplosionTimerCounter + y);
    a |= M(PiranhaPlantDownYPos + y);
    if (z)
        goto ExitRp;
    x = M(VRAM_Buffer1_Offset);
    compare(x, 0x20);
    if (c)
        goto ExitRp;
    a = M(ExplosionTimerCounter + y);
    pha();
    pha();
    JSR(SetupPlatformRope, 378);
    a = M(GameModeValue);
    writeData(VRAM_Buffer1 + x, a);
    a = M(TitleScreenModeValue);
    writeData(0x0302 + x, a);
    a = 0x02;
    writeData(0x0303 + x, a);
    a = M(ExplosionTimerCounter + y);
    if (n)
        goto EraseR1;
    a = 0xa2;
    writeData(0x0304 + x, a);
    a = 0xa3;
    writeData(0x0305 + x, a);
    goto OtherRope;

EraseR1:
    a = 0x24;
    writeData(0x0304 + x, a);
    writeData(0x0305 + x, a);

OtherRope:
    a = M(Enemy_State + y);
    y = a;
    pla();
    a ^= 0xff;
    JSR(SetupPlatformRope, 379);
    a = M(GameModeValue);
    writeData(0x0306 + x, a);
    a = M(TitleScreenModeValue);
    writeData(0x0307 + x, a);
    a = 0x02;
    writeData(0x0308 + x, a);
    pla();
    if (!n)
        goto EraseR2;
    a = 0xa2;
    writeData(0x0309 + x, a);
    a = 0xa3;
    writeData(0x030a + x, a);
    goto EndRp;

EraseR2:
    a = 0x24;
    writeData(0x0309 + x, a);
    writeData(0x030a + x, a);

EndRp:
    a = 0x00;
    writeData(0x030b + x, a);
    a = M(VRAM_Buffer1_Offset);
    c = 0;
    a += 0x0a;
    writeData(VRAM_Buffer1_Offset, a);

ExitRp:
    x = M(BulletBill_FrenzyVar);
    goto Return;

SetupPlatformRope:
    pha();
    a = M(Enemy_X_Position + y);
    c = 0;
    a += 0x08;
    x = M(SecondaryHardMode);
    if (!z)
        goto GetLRp;
    c = 0;
    a += 0x10;

GetLRp:
    pha();
    a = M(Enemy_PageLoc + y);
    a += 0x00;
    writeData(VictoryModeValue, a);
    pla();
    a &= 0xf0;
    a >>= 1;
    a >>= 1;
    a >>= 1;
    writeData(TitleScreenModeValue, a);
    x = M(Enemy_Y_Position + y);
    pla();
    if (!n)
        goto GetHRp;
    a = x;
    c = 0;
    a += 0x08;
    x = a;

GetHRp:
    a = x;
    x = M(VRAM_Buffer1_Offset);
    a <<= 1;
    a.rol();
    pha();
    a.rol();
    a &= 0x03;
    a |= 0x20;
    writeData(GameModeValue, a);
    a = M(VictoryModeValue);
    a &= 0x01;
    a <<= 1;
    a <<= 1;
    a |= M(GameModeValue);
    writeData(GameModeValue, a);
    pla();
    a &= 0xe0;
    c = 0;
    a += M(TitleScreenModeValue);
    writeData(TitleScreenModeValue, a);
    a = M(Enemy_Y_Position + y);
    compare(a, 0xe8);
    if (!c)
        goto ExPRp;
    a = M(TitleScreenModeValue);
    a &= 0xbf;
    writeData(TitleScreenModeValue, a);

ExPRp:
    goto Return;

InitPlatformFall:
    a = y;
    x = a;
    JSR(GetEnemyOffscreenBits, 380);
    a = 0x06;
    JSR(SetupFloateyNumber, 381);
    a = M(Player_Rel_XPos);
    writeData(FloateyNum_X_Pos + x, a);
    a = M(Player_Y_Position);
    writeData(FloateyNum_Y_Pos + x, a);
    a = 0x01;
    writeData(Enemy_MovingDir + x, a);

StopPlatforms:
    JSR(InitVStf, 382);
    writeData(ExplosionTimerCounter + y, a);
    writeData(PiranhaPlantDownYPos + y, a);
    goto Return;

PlatformFall:
    a = y;
    pha();
    JSR(MoveFallingPlatform, 383);
    pla();
    x = a;
    JSR(MoveFallingPlatform, 384);
    x = M(BulletBill_FrenzyVar);
    a = M(HammerThrowingTimer + x);
    if (n)
        goto ExPF;
    x = a;
    JSR(PositionPlayerOnVPlat, 385);

ExPF:
    x = M(BulletBill_FrenzyVar);
    goto Return;
    // ;--------------------------------

YMovingPlatform:
    a = M(ExplosionTimerCounter + x);
    a |= M(PiranhaPlantDownYPos + x);
    if (!z)
        goto ChkYCenterPos;
    writeData(PiranhaPlantUpYPos + x, a);
    a = M(Enemy_Y_Position + x);
    compare(a, M(RedPTroopaOrigXPos + x));
    if (c)
        goto ChkYCenterPos;
    a = M(TallEnemy);
    a &= 0x07;
    if (!z)
        goto SkipIY;
    ++M(Enemy_Y_Position + x);

SkipIY:
    goto ChkYPCollision;

ChkYCenterPos:
    a = M(Enemy_Y_Position + x);
    compare(a, M(ExplosionGfxCounter + x));
    if (!c)
        goto YMDown;
    JSR(MovePlatformUp, 386);
    goto ChkYPCollision;

YMDown:
    JSR(MovePlatformDown, 387);

ChkYPCollision:
    a = M(HammerThrowingTimer + x);
    if (n)
        goto ExYPl;
    JSR(PositionPlayerOnVPlat, 388);

ExYPl:
    goto Return;
    // ;--------------------------------
    // ;$00 - used as adder to position player hotizontally

XMovingPlatform:
    a = 0x0e;
    JSR(XMoveCntr_Platform, 389);
    JSR(MoveWithXMCntrs, 390);
    a = M(HammerThrowingTimer + x);
    if (n)
        goto ExXMP;

PositionPlayerOnHPlat:
    a = M(Player_X_Position);
    c = 0;
    a += M(TitleScreenModeValue);
    writeData(Player_X_Position, a);
    a = M(Player_PageLoc);
    y = M(TitleScreenModeValue);
    if (n)
        goto PPHSubt;
    a += 0x00;
    goto SetPVar;

PPHSubt:
    a -= 0x00;

SetPVar:
    writeData(Player_PageLoc, a);
    writeData(Platform_X_Scroll, y);
    JSR(PositionPlayerOnVPlat, 391);

ExXMP:
    goto Return;
    // ;--------------------------------

DropPlatform:
    a = M(HammerThrowingTimer + x);
    if (n)
        goto ExDPl;
    JSR(MoveDropPlatform, 392);
    JSR(PositionPlayerOnVPlat, 393);

ExDPl:
    goto Return;
    // ;--------------------------------
    // ;$00 - residual value from sub

RightPlatform:
    JSR(MoveEnemyHorizontally, 394);
    writeData(TitleScreenModeValue, a);
    a = M(HammerThrowingTimer + x);
    if (n)
        goto ExRPl;
    a = 0x10;
    writeData(ExplosionGfxCounter + x, a);
    JSR(PositionPlayerOnHPlat, 395);

ExRPl:
    goto Return;
    // ;--------------------------------

MoveLargeLiftPlat:
    JSR(MoveLiftPlatforms, 396);
    goto ChkYPCollision;

MoveSmallPlatform:
    JSR(MoveLiftPlatforms, 397);
    goto ChkSmallPlatCollision;

MoveLiftPlatforms:
    a = M(TimerControl);
    if (!z)
        goto ExLiftP;
    a = M(PiranhaPlantUpYPos + x);
    c = 0;
    a += M(PiranhaPlantDownYPos + x);
    writeData(PiranhaPlantUpYPos + x, a);
    a = M(Enemy_Y_Position + x);
    a += M(ExplosionTimerCounter + x);
    writeData(Enemy_Y_Position + x, a);
    goto Return;

ChkSmallPlatCollision:
    a = M(HammerThrowingTimer + x);
    if (z)
        goto ExLiftP;
    JSR(PositionPlayerOnS_Plat, 398);

ExLiftP:
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$00 - page location of extended left boundary
    // ;$01 - extended left boundary position
    // ;$02 - page location of extended right boundary
    // ;$03 - extended right boundary position

OffscreenBoundsCheck:
    a = M(Fireworks + x);
    compare(a, 0x14);
    if (z)
        goto ExScrnBd;
    a = M(ScreenLeft_X_Pos);
    y = M(Fireworks + x);
    compare(y, 0x05);
    if (z)
        goto LimitB;
    compare(y, 0x0d);
    if (!z)
        goto ExtendLB;

LimitB:
    a += 0x38;

ExtendLB:
    a -= 0x48;
    writeData(GameModeValue, a);
    a = M(ScreenLeft_PageLoc);
    a -= 0x00;
    writeData(TitleScreenModeValue, a);
    a = M(ScreenRight_X_Pos);
    a += 0x48;
    writeData(GameOverModeValue, a);
    a = M(ScreenRight_PageLoc);
    a += 0x00;
    writeData(VictoryModeValue, a);
    a = M(Enemy_X_Position + x);
    compare(a, M(GameModeValue));
    a = M(Enemy_PageLoc + x);
    a -= M(TitleScreenModeValue);
    if (n)
        goto TooFar;
    a = M(Enemy_X_Position + x);
    compare(a, M(GameOverModeValue));
    a = M(Enemy_PageLoc + x);
    a -= M(VictoryModeValue);
    if (n)
        goto ExScrnBd;
    a = M(Enemy_State + x);
    compare(a, 0x05);
    if (z)
        goto ExScrnBd;
    compare(y, 0x0d);
    if (z)
        goto ExScrnBd;
    compare(y, 0x30);
    if (z)
        goto ExScrnBd;
    compare(y, 0x31);
    if (z)
        goto ExScrnBd;
    compare(y, 0x32);
    if (z)
        goto ExScrnBd;

TooFar:
    JSR(EraseEnemyObject, 399);

ExScrnBd:
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;some unused space
    // ;-------------------------------------------------------------------------------------
    // ;$01 - enemy buffer offset

FireballEnemyCollision:
    a = M(Fireball_State + x);
    if (z)
        goto ExitFBallEnemy;
    a <<= 1;
    if (c)
        goto ExitFBallEnemy;
    a = M(TallEnemy);
    a >>= 1;
    if (c)
        goto ExitFBallEnemy;
    a = x;
    a <<= 1;
    a <<= 1;
    c = 0;
    a += 0x1c;
    y = a;
    x = 0x04;

FireballEnemyCDLoop:
    writeData(GameModeValue, x);
    a = y;
    pha();
    a = M(Enemy_State + x);
    a &= 0x20;
    if (!z)
        goto NoFToECol;
    a = M(RedParatroopa + x);
    if (z)
        goto NoFToECol;
    a = M(Fireworks + x);
    compare(a, 0x24);
    if (!c)
        goto GoombaDie;
    compare(a, 0x2b);
    if (!c)
        goto NoFToECol;

GoombaDie:
    compare(a, 0x06);
    if (!z)
        goto NotGoomba;
    a = M(Enemy_State + x);
    compare(a, 0x02);
    if (c)
        goto NoFToECol;

NotGoomba:
    a = M(EnemyOffscrBitsMasked + x);
    if (!z)
        goto NoFToECol;
    a = x;
    a <<= 1;
    a <<= 1;
    c = 0;
    a += 0x04;
    x = a;
    JSR(SprObjectCollisionCore, 400);
    x = M(BulletBill_FrenzyVar);
    if (!c)
        goto NoFToECol;
    a = 0x80;
    writeData(Fireball_State + x, a);
    x = M(GameModeValue);
    JSR(HandleEnemyFBallCol, 401);

NoFToECol:
    pla();
    y = a;
    x = M(GameModeValue);
    --x;
    if (!n)
        goto FireballEnemyCDLoop;

ExitFBallEnemy:
    x = M(BulletBill_FrenzyVar);
    goto Return;

HandleEnemyFBallCol:
    JSR(RelativeEnemyPosition, 402);
    x = M(GameModeValue);
    a = M(RedParatroopa + x);
    if (!n)
        goto ChkBuzzyBeetle;
    a &= 0x0f;
    x = a;
    a = M(Fireworks + x);
    compare(a, 0x2d);
    if (z)
        goto HurtBowser;
    x = M(GameModeValue);

ChkBuzzyBeetle:
    a = M(Fireworks + x);
    compare(a, 0x02);
    if (z)
        goto ExHCF;
    compare(a, 0x2d);
    if (!z)
        goto ChkOtherEnemies;

HurtBowser:
    --M(BowserHitPoints);
    if (!z)
        goto ExHCF;
    JSR(InitVStf, 403);
    writeData(ExplosionGfxCounter + x, a);
    writeData(EnemyFrenzyBuffer, a);
    a = 0xfe;
    writeData(ExplosionTimerCounter + x, a);
    y = M(WorldNumber);
    a = M(BowserIdentities + y);
    writeData(Fireworks + x, a);
    a = 0x20;
    compare(y, 0x03);
    if (c)
        goto SetDBSte;
    a |= 0x03;

SetDBSte:
    writeData(Enemy_State + x, a);
    a = 0x80;
    writeData(Square2SoundQueue, a);
    x = M(GameModeValue);
    a = 0x09;
    if (!z)
        goto EnemySmackScore;

ChkOtherEnemies:
    compare(a, 0x08);
    if (z)
        goto ExHCF;
    compare(a, 0x0c);
    if (z)
        goto ExHCF;
    compare(a, 0x15);
    if (c)
        goto ExHCF;

ShellOrBlockDefeat:
    a = M(Fireworks + x);
    compare(a, 0x0d);
    if (!z)
        goto StnE;
    a = M(Enemy_Y_Position + x);
    a += 0x18;
    writeData(Enemy_Y_Position + x, a);

StnE:
    JSR(ChkToStunEnemies, 404);
    a = M(Enemy_State + x);
    a &= 0x1f;
    a |= 0x20;
    writeData(Enemy_State + x, a);
    a = 0x02;
    y = M(Fireworks + x);
    compare(y, 0x05);
    if (!z)
        goto GoombaPoints;
    a = 0x06;

GoombaPoints:
    compare(y, 0x06);
    if (!z)
        goto EnemySmackScore;
    a = 0x01;

EnemySmackScore:
    JSR(SetupFloateyNumber, 405);
    a = 0x08;
    writeData(Square1SoundQueue, a);

ExHCF:
    goto Return;
    // ;-------------------------------------------------------------------------------------

PlayerHammerCollision:
    a = M(TallEnemy);
    a >>= 1;
    if (!c)
        goto ExPHC;
    a = M(TimerControl);
    a |= M(Misc_OffscreenBits);
    if (!z)
        goto ExPHC;
    a = x;
    a <<= 1;
    a <<= 1;
    c = 0;
    a += 0x24;
    y = a;
    JSR(PlayerCollisionCore, 406);
    x = M(BulletBill_FrenzyVar);
    if (!c)
        goto ClHCol;
    a = M(Misc_Collision_Flag + x);
    if (!z)
        goto ExPHC;
    a = 0x01;
    writeData(Misc_Collision_Flag + x, a);
    a = M(Misc_X_Speed + x);
    a ^= 0xff;
    c = 0;
    a += 0x01;
    writeData(Misc_X_Speed + x, a);
    a = M(StarInvincibleTimer);
    if (!z)
        goto ExPHC;
    goto InjurePlayer;

ClHCol:
    a = 0x00;
    writeData(Misc_Collision_Flag + x, a);

ExPHC:
    goto Return;
    // ;-------------------------------------------------------------------------------------

HandlePowerUpCollision:
    JSR(EraseEnemyObject, 407);
    a = 0x06;
    JSR(SetupFloateyNumber, 408);
    a = 0x20;
    writeData(Square2SoundQueue, a);
    a = M(PowerUpType);
    compare(a, 0x02);
    if (!c)
        goto Shroom_Flower_PUp;
    compare(a, 0x03);
    if (z)
        goto SetFor1Up;
    a = 0x23;
    writeData(StarInvincibleTimer, a);
    a = 0x40;
    writeData(AreaMusicQueue, a);
    goto Return;

Shroom_Flower_PUp:
    a = M(PlayerStatus);
    if (z)
        goto UpToSuper;
    compare(a, 0x01);
    if (!z)
        goto NoPUp;
    x = M(BulletBill_FrenzyVar);
    a = 0x02;
    writeData(PlayerStatus, a);
    JSR(GetPlayerColors, 409);
    x = M(BulletBill_FrenzyVar);
    a = 0x0c;
    goto UpToFiery;

SetFor1Up:
    a = 0x0b;
    writeData(FloateyNum_Control + x, a);
    goto Return;

UpToSuper:
    a = 0x01;
    writeData(PlayerStatus, a);
    a = 0x09;

UpToFiery:
    y = 0x00;
    JSR(SetPRout, 410);

NoPUp:
    goto Return;
    // ;--------------------------------

PlayerEnemyCollision:
    a = M(TallEnemy);
    a >>= 1;
    if (c)
        goto NoPUp;
    JSR(CheckPlayerVertical, 411);
    if (c)
        goto NoPECol;
    a = M(EnemyOffscrBitsMasked + x);
    if (!z)
        goto NoPECol;
    a = M(GreenParatroopaJump);
    compare(a, 0x08);
    if (!z)
        goto NoPECol;
    a = M(Enemy_State + x);
    a &= 0x20;
    if (!z)
        goto NoPECol;
    JSR(GetEnemyBoundBoxOfs, 412);
    JSR(PlayerCollisionCore, 413);
    x = M(BulletBill_FrenzyVar);
    if (c)
        goto CheckForPUpCollision;
    a = M(Enemy_CollisionBits + x);
    a &= 0xfe;
    writeData(Enemy_CollisionBits + x, a);

NoPECol:
    goto Return;

CheckForPUpCollision:
    y = M(Fireworks + x);
    compare(y, 0x2e);
    if (!z)
        goto EColl;
    goto HandlePowerUpCollision;

EColl:
    a = M(StarInvincibleTimer);
    if (z)
        goto HandlePECollisions;
    goto ShellOrBlockDefeat;

HandlePECollisions:
    a = M(Enemy_CollisionBits + x);
    a &= 0x01;
    a |= M(EnemyOffscrBitsMasked + x);
    if (!z)
        goto ExPEC;
    a = 0x01;
    a |= M(Enemy_CollisionBits + x);
    writeData(Enemy_CollisionBits + x, a);
    compare(y, 0x12);
    if (z)
        goto ChkForPlayerInjury;
    compare(y, 0x0d);
    if (z)
        goto InjurePlayer;
    compare(y, 0x0c);
    if (z)
        goto InjurePlayer;
    compare(y, 0x33);
    if (z)
        goto ChkForPlayerInjury;
    compare(y, 0x15);
    if (c)
        goto InjurePlayer;
    a = M(AreaType);
    if (z)
        goto InjurePlayer;
    a = M(Enemy_State + x);
    a <<= 1;
    if (c)
        goto ChkForPlayerInjury;
    a = M(Enemy_State + x);
    a &= 0x07;
    compare(a, 0x02);
    if (!c)
        goto ChkForPlayerInjury;
    a = M(Fireworks + x);
    compare(a, 0x06);
    if (z)
        goto ExPEC;
    a = 0x08;
    writeData(Square1SoundQueue, a);
    a = M(Enemy_State + x);
    a |= 0x80;
    writeData(Enemy_State + x, a);
    JSR(EnemyFacePlayer, 414);
    a = M(KickedShellXSpdData + y);
    writeData(ExplosionGfxCounter + x, a);
    a = 0x03;
    c = 0;
    a += M(StompChainCounter);
    y = M(EnemyIntervalTimer + x);
    compare(y, 0x03);
    if (c)
        goto KSPts;
    a = M(KickedShellPtsData + y);

KSPts:
    JSR(SetupFloateyNumber, 415);

ExPEC:
    goto Return;

ChkForPlayerInjury:
    a = M(Player_Y_Speed);
    if (n)
        goto ChkInj;
    if (!z)
        goto EnemyStomped;

ChkInj:
    a = M(Fireworks + x);
    compare(a, 0x07);
    if (!c)
        goto ChkETmrs;
    a = M(Player_Y_Position);
    c = 0;
    a += 0x0c;
    compare(a, M(Enemy_Y_Position + x));
    if (!c)
        goto EnemyStomped;

ChkETmrs:
    a = M(StompTimer);
    if (!z)
        goto EnemyStomped;
    a = M(InjuryTimer);
    if (!z)
        goto ExInjColRoutines;
    a = M(Player_Rel_XPos);
    compare(a, M(Enemy_Rel_XPos));
    if (!c)
        goto TInjE;
    goto ChkEnemyFaceRight;

TInjE:
    a = M(Enemy_MovingDir + x);
    compare(a, 0x01);
    if (!z)
        goto InjurePlayer;
    goto LInj;

InjurePlayer:
    a = M(InjuryTimer);
    if (!z)
        goto ExInjColRoutines;

ForceInjury:
    x = M(PlayerStatus);
    if (z)
        goto KillPlayer;
    writeData(PlayerStatus, a);
    a = 0x08;
    writeData(InjuryTimer, a);
    a <<= 1;
    writeData(Square1SoundQueue, a);
    JSR(GetPlayerColors, 416);
    a = 0x0a;

SetKRout:
    y = 0x01;

SetPRout:
    writeData(GreenParatroopaJump, a);
    writeData(Player_State, y);
    y = 0xff;
    writeData(TimerControl, y);
    ++y;
    writeData(ScrollAmount, y);

ExInjColRoutines:
    x = M(BulletBill_FrenzyVar);
    goto Return;

KillPlayer:
    writeData(Player_X_Speed, x);
    ++x;
    writeData(EventMusicQueue, x);
    a = 0xfc;
    writeData(Player_Y_Speed, a);
    a = 0x0b;
    if (!z)
        goto SetKRout;

EnemyStomped:
    a = M(Fireworks + x);
    compare(a, 0x12);
    if (z)
        goto InjurePlayer;
    a = 0x04;
    writeData(Square1SoundQueue, a);
    a = M(Fireworks + x);
    y = 0x00;
    compare(a, 0x14);
    if (z)
        goto EnemyStompedPts;
    compare(a, 0x08);
    if (z)
        goto EnemyStompedPts;
    compare(a, 0x33);
    if (z)
        goto EnemyStompedPts;
    compare(a, 0x0c);
    if (z)
        goto EnemyStompedPts;
    ++y;
    compare(a, 0x05);
    if (z)
        goto EnemyStompedPts;
    ++y;
    compare(a, 0x11);
    if (z)
        goto EnemyStompedPts;
    ++y;
    compare(a, 0x07);
    if (!z)
        goto ChkForDemoteKoopa;

EnemyStompedPts:
    a = M(StompedEnemyPtsData + y);
    JSR(SetupFloateyNumber, 417);
    a = M(Enemy_MovingDir + x);
    pha();
    JSR(SetStun, 418);
    pla();
    writeData(Enemy_MovingDir + x, a);
    a = 0x20;
    writeData(Enemy_State + x, a);
    JSR(InitVStf, 419);
    writeData(ExplosionGfxCounter + x, a);
    a = 0xfd;
    writeData(Player_Y_Speed, a);
    goto Return;

ChkForDemoteKoopa:
    compare(a, 0x09);
    if (!c)
        goto HandleStompedShellE;
    a &= 0x01;
    writeData(Fireworks + x, a);
    y = 0x00;
    writeData(Enemy_State + x, y);
    a = 0x03;
    JSR(SetupFloateyNumber, 420);
    JSR(InitVStf, 421);
    JSR(EnemyFacePlayer, 422);
    a = M(DemotedKoopaXSpdData + y);
    writeData(ExplosionGfxCounter + x, a);
    goto SBnce;

HandleStompedShellE:
    a = 0x04;
    writeData(Enemy_State + x, a);
    ++M(StompChainCounter);
    a = M(StompChainCounter);
    c = 0;
    a += M(StompTimer);
    JSR(SetupFloateyNumber, 423);
    ++M(StompTimer);
    y = M(PrimaryHardMode);
    a = M(RevivalRateData + y);
    writeData(EnemyIntervalTimer + x, a);

SBnce:
    a = 0xfc;
    writeData(Player_Y_Speed, a);
    goto Return;

ChkEnemyFaceRight:
    a = M(Enemy_MovingDir + x);
    compare(a, 0x01);
    if (!z)
        goto LInj;
    goto InjurePlayer;

LInj:
    JSR(EnemyTurnAround, 424);
    goto InjurePlayer;

EnemyFacePlayer:
    y = 0x01;
    JSR(PlayerEnemyDiff, 425);
    if (!n)
        goto SFcRt;
    ++y;

SFcRt:
    writeData(Enemy_MovingDir + x, y);
    --y;
    goto Return;

SetupFloateyNumber:
    writeData(FloateyNum_Control + x, a);
    a = 0x30;
    writeData(FloateyNum_Timer + x, a);
    a = M(Enemy_Y_Position + x);
    writeData(FloateyNum_Y_Pos + x, a);
    a = M(Enemy_Rel_XPos);
    writeData(FloateyNum_X_Pos + x, a);

ExSFN:
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$01 - used to hold enemy offset for second enemy

EnemiesCollision:
    a = M(TallEnemy);
    a >>= 1;
    if (!c)
        goto ExSFN;
    a = M(AreaType);
    if (z)
        goto ExSFN;
    a = M(Fireworks + x);
    compare(a, 0x15);
    if (c)
        goto ExitECRoutine;
    compare(a, 0x11);
    if (z)
        goto ExitECRoutine;
    compare(a, 0x0d);
    if (z)
        goto ExitECRoutine;
    a = M(EnemyOffscrBitsMasked + x);
    if (!z)
        goto ExitECRoutine;
    JSR(GetEnemyBoundBoxOfs, 426);
    --x;
    if (n)
        goto ExitECRoutine;

ECLoop:
    writeData(GameModeValue, x);
    a = y;
    pha();
    a = M(RedParatroopa + x);
    if (z)
        goto ReadyNextEnemy;
    a = M(Fireworks + x);
    compare(a, 0x15);
    if (c)
        goto ReadyNextEnemy;
    compare(a, 0x11);
    if (z)
        goto ReadyNextEnemy;
    compare(a, 0x0d);
    if (z)
        goto ReadyNextEnemy;
    a = M(EnemyOffscrBitsMasked + x);
    if (!z)
        goto ReadyNextEnemy;
    a = x;
    a <<= 1;
    a <<= 1;
    c = 0;
    a += 0x04;
    x = a;
    JSR(SprObjectCollisionCore, 427);
    x = M(BulletBill_FrenzyVar);
    y = M(GameModeValue);
    if (!c)
        goto NoEnemyCollision;
    a = M(Enemy_State + x);
    a |= M(Enemy_State + y);
    a &= 0x80;
    if (!z)
        goto YesEC;
    a = M(Enemy_CollisionBits + y);
    a &= M(SetBitsMask + x);
    if (!z)
        goto ReadyNextEnemy;
    a = M(Enemy_CollisionBits + y);
    a |= M(SetBitsMask + x);
    writeData(Enemy_CollisionBits + y, a);

YesEC:
    JSR(ProcEnemyCollisions, 428);
    goto ReadyNextEnemy;

NoEnemyCollision:
    a = M(Enemy_CollisionBits + y);
    a &= M(ClearBitsMask + x);
    writeData(Enemy_CollisionBits + y, a);

ReadyNextEnemy:
    pla();
    y = a;
    x = M(GameModeValue);
    --x;
    if (!n)
        goto ECLoop;

ExitECRoutine:
    x = M(BulletBill_FrenzyVar);
    goto Return;

ProcEnemyCollisions:
    a = M(Enemy_State + y);
    a |= M(Enemy_State + x);
    a &= 0x20;
    if (!z)
        goto ExitProcessEColl;
    a = M(Enemy_State + x);
    compare(a, 0x06);
    if (!c)
        goto ProcSecondEnemyColl;
    a = M(Fireworks + x);
    compare(a, 0x05);
    if (z)
        goto ExitProcessEColl;
    a = M(Enemy_State + y);
    a <<= 1;
    if (!c)
        goto ShellCollisions;
    a = 0x06;
    JSR(SetupFloateyNumber, 429);
    JSR(ShellOrBlockDefeat, 430);
    y = M(GameModeValue);

ShellCollisions:
    a = y;
    x = a;
    JSR(ShellOrBlockDefeat, 431);
    x = M(BulletBill_FrenzyVar);
    a = M(ShellChainCounter + x);
    c = 0;
    a += 0x04;
    x = M(GameModeValue);
    JSR(SetupFloateyNumber, 432);
    x = M(BulletBill_FrenzyVar);
    ++M(ShellChainCounter + x);

ExitProcessEColl:
    goto Return;

ProcSecondEnemyColl:
    a = M(Enemy_State + y);
    compare(a, 0x06);
    if (!c)
        goto MoveEOfs;
    a = M(Fireworks + y);
    compare(a, 0x05);
    if (z)
        goto ExitProcessEColl;
    JSR(ShellOrBlockDefeat, 433);
    y = M(GameModeValue);
    a = M(ShellChainCounter + y);
    c = 0;
    a += 0x04;
    x = M(BulletBill_FrenzyVar);
    JSR(SetupFloateyNumber, 434);
    x = M(GameModeValue);
    ++M(ShellChainCounter + x);
    goto Return;

MoveEOfs:
    a = y;
    x = a;
    JSR(EnemyTurnAround, 435);
    x = M(BulletBill_FrenzyVar);

EnemyTurnAround:
    a = M(Fireworks + x);
    compare(a, 0x0d);
    if (z)
        goto ExTA;
    compare(a, 0x11);
    if (z)
        goto ExTA;
    compare(a, 0x05);
    if (z)
        goto ExTA;
    compare(a, 0x12);
    if (z)
        goto RXSpd;
    compare(a, 0x0e);
    if (z)
        goto RXSpd;
    compare(a, 0x07);
    if (c)
        goto ExTA;

RXSpd:
    a = M(ExplosionGfxCounter + x);
    a ^= 0xff;
    y = a;
    ++y;
    writeData(ExplosionGfxCounter + x, y);
    a = M(Enemy_MovingDir + x);
    a ^= 0x03;
    writeData(Enemy_MovingDir + x, a);

ExTA:
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$00 - vertical position of platform

LargePlatformCollision:
    a = 0xff;
    writeData(HammerThrowingTimer + x, a);
    a = M(TimerControl);
    if (!z)
        goto ExLPC;
    a = M(Enemy_State + x);
    if (n)
        goto ExLPC;
    a = M(Fireworks + x);
    compare(a, 0x24);
    if (!z)
        goto ChkForPlayerC_LargeP;
    a = M(Enemy_State + x);
    x = a;
    JSR(ChkForPlayerC_LargeP, 436);

ChkForPlayerC_LargeP:
    JSR(CheckPlayerVertical, 437);
    if (c)
        goto ExLPC;
    a = x;
    JSR(GetEnemyBoundBoxOfsArg, 438);
    a = M(Enemy_Y_Position + x);
    writeData(TitleScreenModeValue, a);
    a = x;
    pha();
    JSR(PlayerCollisionCore, 439);
    pla();
    x = a;
    if (!c)
        goto ExLPC;
    JSR(ProcLPlatCollisions, 440);

ExLPC:
    x = M(BulletBill_FrenzyVar);
    goto Return;
    // ;--------------------------------
    // ;$00 - counter for bounding boxes

SmallPlatformCollision:
    a = M(TimerControl);
    if (!z)
        goto ExSPC;
    writeData(HammerThrowingTimer + x, a);
    JSR(CheckPlayerVertical, 441);
    if (c)
        goto ExSPC;
    a = 0x02;
    writeData(TitleScreenModeValue, a);

ChkSmallPlatLoop:
    x = M(BulletBill_FrenzyVar);
    JSR(GetEnemyBoundBoxOfs, 442);
    a &= 0x02;
    if (!z)
        goto ExSPC;
    a = M(BoundingBox_UL_YPos + y);
    compare(a, 0x20);
    if (!c)
        goto MoveBoundBox;
    JSR(PlayerCollisionCore, 443);
    if (c)
        goto ProcSPlatCollisions;

MoveBoundBox:
    a = M(BoundingBox_UL_YPos + y);
    c = 0;
    a += 0x80;
    writeData(BoundingBox_UL_YPos + y, a);
    a = M(BoundingBox_DR_YPos + y);
    c = 0;
    a += 0x80;
    writeData(BoundingBox_DR_YPos + y, a);
    --M(TitleScreenModeValue);
    if (!z)
        goto ChkSmallPlatLoop;

ExSPC:
    x = M(BulletBill_FrenzyVar);
    goto Return;
    // ;--------------------------------

ProcSPlatCollisions:
    x = M(BulletBill_FrenzyVar);

ProcLPlatCollisions:
    a = M(BoundingBox_DR_YPos + y);
    c = 1;
    a -= M(BoundingBox_UL_YPos);
    compare(a, 0x04);
    if (c)
        goto ChkForTopCollision;
    a = M(Player_Y_Speed);
    if (!n)
        goto ChkForTopCollision;
    a = 0x01;
    writeData(Player_Y_Speed, a);

ChkForTopCollision:
    a = M(BoundingBox_DR_YPos);
    c = 1;
    a -= M(BoundingBox_UL_YPos + y);
    compare(a, 0x06);
    if (c)
        goto PlatformSideCollisions;
    a = M(Player_Y_Speed);
    if (n)
        goto PlatformSideCollisions;
    a = M(TitleScreenModeValue);
    y = M(Fireworks + x);
    compare(y, 0x2b);
    if (z)
        goto SetCollisionFlag;
    compare(y, 0x2c);
    if (z)
        goto SetCollisionFlag;
    a = x;

SetCollisionFlag:
    x = M(BulletBill_FrenzyVar);
    writeData(HammerThrowingTimer + x, a);
    a = 0x00;
    writeData(Player_State, a);
    goto Return;

PlatformSideCollisions:
    a = 0x01;
    writeData(TitleScreenModeValue, a);
    a = M(BoundingBox_LR_Corner);
    c = 1;
    a -= M(BoundingBox_UL_Corner + y);
    compare(a, 0x08);
    if (!c)
        goto SideC;
    ++M(TitleScreenModeValue);
    a = M(BoundingBox_LR_Corner + y);
    c = 0;
    a -= M(BoundingBox_UL_Corner);
    compare(a, 0x09);
    if (c)
        goto NoSideC;

SideC:
    JSR(ImpedePlayerMove, 444);

NoSideC:
    x = M(BulletBill_FrenzyVar);
    goto Return;
    // ;-------------------------------------------------------------------------------------

PositionPlayerOnS_Plat:
    y = a;
    a = M(Enemy_Y_Position + x);
    c = 0;
    a += M(0xdc16 + y);
    bit(M(0xcfb5));
    goto loc_DC23;

PositionPlayerOnVPlat:
    a = M(Enemy_Y_Position + x);
loc_DC23:
    y = M(GreenParatroopaJump);
    compare(y, 0x0b);
    if (z)
        goto ExPlPos;
    y = M(Enemy_Y_HighPos + x);
    compare(y, 0x01);
    if (!z)
        goto ExPlPos;
    c = 1;
    a -= 0x20;
    writeData(Player_Y_Position, a);
    a = y;
    a -= 0x00;
    writeData(Player_Y_HighPos, a);
    a = 0x00;
    writeData(Player_Y_Speed, a);
    writeData(Player_Y_MoveForce, a);

ExPlPos:
    goto Return;
    // ;-------------------------------------------------------------------------------------

CheckPlayerVertical:
    a = M(Player_OffscreenBits);
    compare(a, 0xf0);
    if (c)
        goto ExCPV;
    y = M(Player_Y_HighPos);
    --y;
    if (!z)
        goto ExCPV;
    a = M(Player_Y_Position);
    compare(a, 0xd0);

ExCPV:
    goto Return;
    // ;-------------------------------------------------------------------------------------

GetEnemyBoundBoxOfs:
    a = M(BulletBill_FrenzyVar);

GetEnemyBoundBoxOfsArg:
    a <<= 1;
    a <<= 1;
    c = 0;
    a += 0x04;
    y = a;
    a = M(Enemy_OffscreenBits);
    a &= 0x0f;
    compare(a, 0x0f);
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$00-$01 - used to hold many values, essentially temp variables
    // ;$04 - holds lower nybble of vertical coordinate from block buffer routine
    // ;$eb - used to hold block buffer adder

PlayerBGCollision:
    a = M(DisableCollisionDet);
    if (!z)
        goto ExPBGCol;
    a = M(GreenParatroopaJump);
    compare(a, 0x0b);
    if (z)
        goto ExPBGCol;
    compare(a, 0x04);
    if (!c)
        goto ExPBGCol;
    a = 0x01;
    y = M(SwimmingFlag);
    if (!z)
        goto SetPSte;
    a = M(Player_State);
    if (z)
        goto SetFallS;
    compare(a, 0x03);
    if (!z)
        goto ChkOnScr;

SetFallS:
    a = 0x02;

SetPSte:
    writeData(Player_State, a);

ChkOnScr:
    a = M(Player_Y_HighPos);
    compare(a, 0x01);
    if (!z)
        goto ExPBGCol;
    a = 0xff;
    writeData(Player_CollisionBits, a);
    a = M(Player_Y_Position);
    compare(a, 0xcf);
    if (!c)
        goto ChkCollSize;

ExPBGCol:
    goto Return;

ChkCollSize:
    y = 0x02;
    a = M(CrouchingFlag);
    if (!z)
        goto GBBAdr;
    a = M(PlayerSize);
    if (!z)
        goto GBBAdr;
    --y;
    a = M(SwimmingFlag);
    if (!z)
        goto GBBAdr;
    --y;

GBBAdr:
    a = M(BlockBufferAdderData + y);
    writeData(0x00eb, a);
    y = a;
    x = M(PlayerSize);
    a = M(CrouchingFlag);
    if (z)
        goto HeadChk;
    ++x;

HeadChk:
    a = M(Player_Y_Position);
    compare(a, M(PlayerBGUpperExtent + x));
    if (!c)
        goto DoFootCheck;
    JSR(BlockBufferColli_Head, 445);
    if (z)
        goto DoFootCheck;
    JSR(CheckForCoinMTiles, 446);
    if (c)
        goto AwardTouchedCoin;
    y = M(Player_Y_Speed);
    if (!n)
        goto DoFootCheck;
    y = M(World5);
    compare(y, 0x04);
    if (!c)
        goto DoFootCheck;
    JSR(CheckForSolidMTiles, 447);
    if (c)
        goto SolidOrClimb;
    y = M(AreaType);
    if (z)
        goto NYSpd;
    y = M(BlockBounceTimer);
    if (!z)
        goto NYSpd;
    JSR(PlayerHeadCollision, 448);
    goto DoFootCheck;

SolidOrClimb:
    compare(a, 0x26);
    if (z)
        goto NYSpd;
    a = 0x02;
    writeData(Square1SoundQueue, a);

NYSpd:
    a = 0x01;
    writeData(Player_Y_Speed, a);

DoFootCheck:
    y = M(0x00eb);
    a = M(Player_Y_Position);
    compare(a, 0xcf);
    if (c)
        goto DoPlayerSideCheck;
    JSR(BlockBufferColli_Feet, 449);
    JSR(CheckForCoinMTiles, 450);
    if (c)
        goto AwardTouchedCoin;
    pha();
    JSR(BlockBufferColli_Feet, 451);
    writeData(TitleScreenModeValue, a);
    pla();
    writeData(GameModeValue, a);
    if (!z)
        goto ChkFootMTile;
    a = M(TitleScreenModeValue);
    if (z)
        goto DoPlayerSideCheck;
    JSR(CheckForCoinMTiles, 452);
    if (!c)
        goto ChkFootMTile;

AwardTouchedCoin:
    goto HandleCoinMetatile;

ChkFootMTile:
    JSR(CheckForClimbMTiles, 453);
    if (c)
        goto DoPlayerSideCheck;
    y = M(Player_Y_Speed);
    if (n)
        goto DoPlayerSideCheck;
    compare(a, 0xc5);
    if (!z)
        goto ContChk;
    goto HandleAxeMetatile;

ContChk:
    JSR(ChkInvisibleMTiles, 454);
    if (z)
        goto DoPlayerSideCheck;
    y = M(JumpspringAnimCtrl);
    if (!z)
        goto InitSteP;
    y = M(World5);
    compare(y, 0x05);
    if (!c)
        goto LandPlyr;
    a = M(Player_MovingDir);
    writeData(TitleScreenModeValue, a);
    goto ImpedePlayerMove;

LandPlyr:
    JSR(ChkForLandJumpSpring, 455);
    a = 0xf0;
    a &= M(Player_Y_Position);
    writeData(Player_Y_Position, a);
    JSR(HandlePipeEntry, 456);
    a = 0x00;
    writeData(Player_Y_Speed, a);
    writeData(Player_Y_MoveForce, a);
    writeData(StompChainCounter, a);

InitSteP:
    a = 0x00;
    writeData(Player_State, a);

DoPlayerSideCheck:
    y = M(0x00eb);
    ++y;
    ++y;
    a = 0x02;
    writeData(TitleScreenModeValue, a);

SideCheckLoop:
    ++y;
    writeData(0x00eb, y);
    a = M(Player_Y_Position);
    compare(a, 0x20);
    if (!c)
        goto BHalf;
    compare(a, 0xe4);
    if (c)
        goto ExSCH;
    JSR(BlockBufferColli_Side, 457);
    if (z)
        goto BHalf;
    compare(a, 0x1c);
    if (z)
        goto BHalf;
    compare(a, 0x6b);
    if (z)
        goto BHalf;
    JSR(CheckForClimbMTiles, 458);
    if (!c)
        goto CheckSideMTiles;

BHalf:
    y = M(0x00eb);
    ++y;
    a = M(Player_Y_Position);
    compare(a, 0x08);
    if (!c)
        goto ExSCH;
    compare(a, 0xd0);
    if (c)
        goto ExSCH;
    JSR(BlockBufferColli_Side, 459);
    if (!z)
        goto CheckSideMTiles;
    --M(TitleScreenModeValue);
    if (!z)
        goto SideCheckLoop;

ExSCH:
    goto Return;

CheckSideMTiles:
    JSR(ChkInvisibleMTiles, 460);
    if (z)
        goto ExCSM;
    JSR(CheckForClimbMTiles, 461);
    if (!c)
        goto ContSChk;
    goto HandleClimbing;

ContSChk:
    JSR(CheckForCoinMTiles, 462);
    if (c)
        goto HandleCoinMetatile;
    JSR(ChkJumpspringMetatiles, 463);
    if (!c)
        goto ChkPBtm;
    a = M(JumpspringAnimCtrl);
    if (!z)
        goto ExCSM;
    goto StopPlayerMove;

ChkPBtm:
    y = M(Player_State);
    compare(y, 0x00);
    if (!z)
        goto StopPlayerMove;
    y = M(BulletBill_CannonVar);
    --y;
    if (!z)
        goto StopPlayerMove;
    compare(a, 0x6c);
    if (z)
        goto PipeDwnS;
    compare(a, 0x1f);
    if (!z)
        goto StopPlayerMove;

PipeDwnS:
    a = M(Player_SprAttrib);
    if (!z)
        goto PlyrPipe;
    y = 0x10;
    writeData(Square1SoundQueue, y);

PlyrPipe:
    a |= 0x20;
    writeData(Player_SprAttrib, a);
    a = M(Player_X_Position);
    a &= 0x0f;
    if (z)
        goto ChkGERtn;
    y = 0x00;
    a = M(ScreenLeft_PageLoc);
    if (z)
        goto SetCATmr;
    ++y;

SetCATmr:
    a = M(AreaChangeTimerData + y);
    writeData(ChangeAreaTimer, a);

ChkGERtn:
    a = M(GreenParatroopaJump);
    compare(a, 0x07);
    if (z)
        goto ExCSM;
    compare(a, 0x08);
    if (!z)
        goto ExCSM;
    a = 0x02;
    writeData(GreenParatroopaJump, a);
    goto Return;
    // ;--------------------------------
    // ;$02 - high nybble of vertical coordinate from block buffer
    // ;$04 - low nybble of horizontal coordinate from block buffer
    // ;$06-$07 - block buffer address

StopPlayerMove:
    JSR(ImpedePlayerMove, 464);

ExCSM:
    goto Return;

HandleCoinMetatile:
    JSR(ErACM, 465);
    ++M(CoinTallyFor1Ups);
    goto GiveOneCoin;

HandleAxeMetatile:
    a = 0x00;
    writeData(OperMode_Task, a);
    a = 0x02;
    writeData(OperMode, a);
    a = 0x18;
    writeData(Player_X_Speed, a);

ErACM:
    y = M(VictoryModeValue);
    a = 0x00;
    writeData(W(World7) + y, a);
    goto RemoveCoin_Axe;
    // ;--------------------------------
    // ;$02 - high nybble of vertical coordinate from block buffer
    // ;$04 - low nybble of horizontal coordinate from block buffer
    // ;$06-$07 - block buffer address

HandleClimbing:
    y = M(World5);
    compare(y, 0x06);
    if (!c)
        goto ExHC;
    compare(y, 0x0a);
    if (!c)
        goto ChkForFlagpole;

ExHC:
    goto Return;

ChkForFlagpole:
    compare(a, 0x24);
    if (z)
        goto FlagpoleCollision;
    compare(a, 0x25);
    if (!z)
        goto VineCollision;

FlagpoleCollision:
    a = M(GreenParatroopaJump);
    compare(a, 0x05);
    if (z)
        goto PutPlayerOnVine;
    a = 0x01;
    writeData(BulletBill_CannonVar, a);
    ++M(ScrollLock);
    a = M(GreenParatroopaJump);
    compare(a, 0x04);
    if (z)
        goto RunFR;
    a = 0x33;
    JSR(KillEnemies, 466);
    a = 0x80;
    writeData(EventMusicQueue, a);
    a >>= 1;
    writeData(FlagpoleSoundQueue, a);
    x = 0x04;
    a = M(Player_Y_Position);
    writeData(FlagpoleCollisionYPos, a);

ChkFlagpoleYPosLoop:
    compare(a, M(FlagpoleYPosData + x));
    if (c)
        goto MtchF;
    --x;
    if (!z)
        goto ChkFlagpoleYPosLoop;

MtchF:
    writeData(FlagpoleScore, x);

RunFR:
    a = 0x04;
    writeData(GreenParatroopaJump, a);
    goto PutPlayerOnVine;

VineCollision:
    compare(a, 0x26);
    if (!z)
        goto PutPlayerOnVine;
    a = M(Player_Y_Position);
    compare(a, 0x20);
    if (c)
        goto PutPlayerOnVine;
    a = 0x01;
    writeData(GreenParatroopaJump, a);

PutPlayerOnVine:
    a = 0x03;
    writeData(Player_State, a);
    a = 0x00;
    writeData(Player_X_Speed, a);
    writeData(Player_X_MoveForce, a);
    a = M(Player_X_Position);
    c = 1;
    a -= M(ScreenLeft_X_Pos);
    compare(a, 0x10);
    if (c)
        goto SetVXPl;
    a = 0x02;
    writeData(BulletBill_CannonVar, a);

SetVXPl:
    y = M(BulletBill_CannonVar);
    a = M(World7);
    a <<= 1;
    a <<= 1;
    a <<= 1;
    a <<= 1;
    c = 0;
    a += M(0xde24 + y);
    writeData(Player_X_Position, a);
    a = M(World7);
    if (!z)
        goto ExPVne;
    a = M(ScreenRight_PageLoc);
    c = 0;
    a += M(0xde26 + y);
    writeData(Player_PageLoc, a);

ExPVne:
    goto Return;
    // ;--------------------------------

ChkInvisibleMTiles:
    compare(a, 0x5f);
    if (z)
        goto ExCInvT;
    compare(a, 0x60);

ExCInvT:
    goto Return;
    // ;--------------------------------
    // ;$00-$01 - used to hold bottom right and bottom left metatiles (in that order)
    // ;$00 - used as flag by ImpedePlayerMove to restrict specific movement

ChkForLandJumpSpring:
    JSR(ChkJumpspringMetatiles, 467);
    if (!c)
        goto ExCJSp;
    a = 0x70;
    writeData(VerticalForce, a);
    a = 0xf9;
    writeData(JumpspringForce, a);
    a = 0x03;
    writeData(JumpspringTimer, a);
    a >>= 1;
    writeData(JumpspringAnimCtrl, a);

ExCJSp:
    goto Return;

ChkJumpspringMetatiles:
    compare(a, 0x67);
    if (z)
        goto JSFnd;
    compare(a, 0x68);
    c = 0;
    if (!z)
        goto NoJSFnd;

JSFnd:
    c = 1;

NoJSFnd:
    goto Return;

HandlePipeEntry:
    a = M(RedCheepCheep);
    a &= 0x04;
    if (z)
        goto ExPipeE;
    a = M(TitleScreenModeValue);
    compare(a, 0x11);
    if (!z)
        goto ExPipeE;
    a = M(GameModeValue);
    compare(a, 0x10);
    if (!z)
        goto ExPipeE;
    a = 0x30;
    writeData(ChangeAreaTimer, a);
    a = 0x03;
    writeData(GreenParatroopaJump, a);
    a = 0x10;
    writeData(Square1SoundQueue, a);
    a = 0x20;
    writeData(Player_SprAttrib, a);
    a = M(WarpZoneControl);
    if (z)
        goto ExPipeE;
    a &= 0x03;
    a <<= 1;
    a <<= 1;
    x = a;
    a = M(Player_X_Position);
    compare(a, 0x60);
    if (!c)
        goto GetWNum;
    ++x;
    compare(a, 0xa0);
    if (!c)
        goto GetWNum;
    ++x;

GetWNum:
    y = M(WarpZoneNumbers + x);
    --y;
    writeData(WorldNumber, y);
    x = M(WorldAddrOffsets + y);
    a = M(AreaAddrOffsets + x);
    writeData(AreaPointer, a);
    a = 0x80;
    writeData(EventMusicQueue, a);
    a = 0x00;
    writeData(EntrancePage, a);
    writeData(AreaNumber, a);
    writeData(LevelNumber, a);
    writeData(AltEntranceControl, a);
    ++M(Hidden1UpFlag);
    ++M(FetchNewGameTimerFlag);

ExPipeE:
    goto Return;

ImpedePlayerMove:
    a = 0x00;
    y = M(Player_X_Speed);
    x = M(TitleScreenModeValue);
    --x;
    if (!z)
        goto RImpd;
    ++x;
    compare(y, 0x00);
    if (n)
        goto ExIPM;
    a = 0xff;
    goto NXSpd;

RImpd:
    x = 0x02;
    compare(y, 0x01);
    if (!n)
        goto ExIPM;
    a = 0x01;

NXSpd:
    y = 0x10;
    writeData(SideCollisionTimer, y);
    y = 0x00;
    writeData(Player_X_Speed, y);
    compare(a, 0x00);
    if (!n)
        goto PlatF;
    --y;

PlatF:
    writeData(TitleScreenModeValue, y);
    c = 0;
    a += M(Player_X_Position);
    writeData(Player_X_Position, a);
    a = M(Player_PageLoc);
    a += M(TitleScreenModeValue);
    writeData(Player_PageLoc, a);

ExIPM:
    a = x;
    a ^= 0xff;
    a &= M(Player_CollisionBits);
    writeData(Player_CollisionBits, a);
    goto Return;
    // ;--------------------------------

CheckForSolidMTiles:
    JSR(GetMTileAttrib, 468);
    compare(a, M(SolidMTileUpperExt + x));
    goto Return;

CheckForClimbMTiles:
    JSR(GetMTileAttrib, 469);
    compare(a, M(ClimbMTileUpperExt + x));
    goto Return;

CheckForCoinMTiles:
    compare(a, 0xc2);
    if (z)
        goto CoinSd;
    compare(a, 0xc3);
    if (z)
        goto CoinSd;
    c = 0;
    goto Return;

CoinSd:
    a = 0x01;
    writeData(Square2SoundQueue, a);
    goto Return;

GetMTileAttrib:
    y = a;
    a &= 0xc0;
    a <<= 1;
    a.rol();
    a.rol();
    x = a;
    a = y;

ExEBG:
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$06-$07 - address from block buffer routine

EnemyToBGCollisionDet:
    a = M(Enemy_State + x);
    a &= 0x20;
    if (!z)
        goto ExEBG;
    JSR(SubtEnemyYPos, 470);
    if (!c)
        goto ExEBG;
    y = M(Fireworks + x);
    compare(y, 0x12);
    if (!z)
        goto DoIDCheckBGColl;
    a = M(Enemy_Y_Position + x);
    compare(a, 0x25);
    if (!c)
        goto ExEBG;

DoIDCheckBGColl:
    compare(y, 0x0e);
    if (!z)
        goto HBChk;
    goto EnemyJump;

HBChk:
    compare(y, 0x05);
    if (!z)
        goto CInvu;
    goto HammerBroBGColl;

CInvu:
    compare(y, 0x12);
    if (z)
        goto YesIn;
    compare(y, 0x2e);
    if (z)
        goto YesIn;
    compare(y, 0x07);
    if (c)
        goto ExEBGChk;

YesIn:
    JSR(ChkUnderEnemy, 471);
    if (!z)
        goto HandleEToBGCollision;

NoEToBGCollision:
    goto ChkForRedKoopa;
    // ;--------------------------------
    // ;$02 - vertical coordinate from block buffer routine

HandleEToBGCollision:
    JSR(ChkForNonSolids, 472);
    if (z)
        goto NoEToBGCollision;
    compare(a, 0x23);
    if (!z)
        goto LandEnemyProperly;
    y = M(VictoryModeValue);
    a = 0x00;
    writeData(W(World7) + y, a);
    a = M(Fireworks + x);
    compare(a, 0x15);
    if (c)
        goto ChkToStunEnemies;
    compare(a, 0x06);
    if (!z)
        goto GiveOEPoints;
    JSR(KillEnemyAboveBlock, 473);

GiveOEPoints:
    a = 0x01;
    JSR(SetupFloateyNumber, 474);

ChkToStunEnemies:
    compare(a, 0x09);
    if (!c)
        goto SetStun;
    compare(a, 0x11);
    if (c)
        goto SetStun;
    compare(a, 0x0a);
    if (!c)
        goto Demote;
    compare(a, 0x0d);
    if (!c)
        goto SetStun;

Demote:
    a &= 0x01;
    writeData(Fireworks + x, a);

SetStun:
    a = M(Enemy_State + x);
    a &= 0xf0;
    a |= 0x02;
    writeData(Enemy_State + x, a);
    --M(Enemy_Y_Position + x);
    --M(Enemy_Y_Position + x);
    a = M(Fireworks + x);
    compare(a, 0x07);
    if (z)
        goto SetWYSpd;
    a = 0xfd;
    y = M(AreaType);
    if (!z)
        goto SetNotW;

SetWYSpd:
    a = 0xff;

SetNotW:
    writeData(ExplosionTimerCounter + x, a);
    y = 0x01;
    JSR(PlayerEnemyDiff, 475);
    if (!n)
        goto ChkBBill;
    ++y;

ChkBBill:
    a = M(Fireworks + x);
    compare(a, 0x33);
    if (z)
        goto NoCDirF;
    compare(a, 0x08);
    if (z)
        goto NoCDirF;
    writeData(Enemy_MovingDir + x, y);

NoCDirF:
    --y;
    a = M(EnemyBGCXSpdData + y);
    writeData(ExplosionGfxCounter + x, a);

ExEBGChk:
    goto Return;
    // ;--------------------------------
    // ;$04 - low nybble of vertical coordinate from block buffer routine

LandEnemyProperly:
    a = M(World5);
    c = 1;
    a -= 0x08;
    compare(a, 0x05);
    if (c)
        goto ChkForRedKoopa;
    a = M(Enemy_State + x);
    a &= 0x40;
    if (!z)
        goto LandEnemyInitState;
    a = M(Enemy_State + x);
    a <<= 1;
    if (!c)
        goto ChkLandedEnemyState;

SChkA:
    goto DoEnemySideCheck;

ChkLandedEnemyState:
    a = M(Enemy_State + x);
    if (z)
        goto SChkA;
    compare(a, 0x05);
    if (z)
        goto ProcEnemyDirection;
    compare(a, 0x03);
    if (c)
        goto ExSteChk;
    a = M(Enemy_State + x);
    compare(a, 0x02);
    if (!z)
        goto ProcEnemyDirection;
    a = 0x10;
    y = M(Fireworks + x);
    compare(y, 0x12);
    if (!z)
        goto SetForStn;
    a = 0x00;

SetForStn:
    writeData(EnemyIntervalTimer + x, a);
    a = 0x03;
    writeData(Enemy_State + x, a);
    JSR(EnemyLanding, 476);

ExSteChk:
    goto Return;

ProcEnemyDirection:
    a = M(Fireworks + x);
    compare(a, 0x06);
    if (z)
        goto LandEnemyInitState;
    compare(a, 0x12);
    if (!z)
        goto InvtD;
    a = 0x01;
    writeData(Enemy_MovingDir + x, a);
    a = 0x08;
    writeData(ExplosionGfxCounter + x, a);
    a = M(TallEnemy);
    a &= 0x07;
    if (z)
        goto LandEnemyInitState;

InvtD:
    y = 0x01;
    JSR(PlayerEnemyDiff, 477);
    if (!n)
        goto CNwCDir;
    ++y;

CNwCDir:
    a = y;
    compare(a, M(Enemy_MovingDir + x));
    if (!z)
        goto LandEnemyInitState;
    JSR(ChkForBump_HammerBroJ, 478);

LandEnemyInitState:
    JSR(EnemyLanding, 479);
    a = M(Enemy_State + x);
    a &= 0x80;
    if (!z)
        goto NMovShellFallBit;
    a = 0x00;
    writeData(Enemy_State + x, a);
    goto Return;

NMovShellFallBit:
    a = M(Enemy_State + x);
    a &= 0xbf;
    writeData(Enemy_State + x, a);
    goto Return;
    // ;--------------------------------

ChkForRedKoopa:
    a = M(Fireworks + x);
    compare(a, 0x03);
    if (!z)
        goto Chk2MSBSt;
    a = M(Enemy_State + x);
    if (z)
        goto ChkForBump_HammerBroJ;

Chk2MSBSt:
    a = M(Enemy_State + x);
    y = a;
    a <<= 1;
    if (!c)
        goto GetSteFromD;
    a = M(Enemy_State + x);
    a |= 0x40;
    goto SetD6Ste;

GetSteFromD:
    a = M(EnemyBGCStateData + y);

SetD6Ste:
    writeData(Enemy_State + x, a);
    // ;--------------------------------
    // ;$00 - used to store bitmask (not used but initialized here)
    // ;$eb - used in DoEnemySideCheck as counter and to compare moving directions

DoEnemySideCheck:
    a = M(Enemy_Y_Position + x);
    compare(a, 0x20);
    if (!c)
        goto ExESdeC;
    y = 0x16;
    a = 0x02;
    writeData(0x00eb, a);

SdeCLoop:
    a = M(0x00eb);
    compare(a, M(Enemy_MovingDir + x));
    if (!z)
        goto NextSdeC;
    a = 0x01;
    JSR(BlockBufferChk_Enemy, 480);
    if (z)
        goto NextSdeC;
    JSR(ChkForNonSolids, 481);
    if (!z)
        goto ChkForBump_HammerBroJ;

NextSdeC:
    --M(0x00eb);
    ++y;
    compare(y, 0x18);
    if (!c)
        goto SdeCLoop;

ExESdeC:
    goto Return;

ChkForBump_HammerBroJ:
    compare(x, 0x05);
    if (z)
        goto NoBump;
    a = M(Enemy_State + x);
    a <<= 1;
    if (!c)
        goto NoBump;
    a = 0x02;
    writeData(Square1SoundQueue, a);

NoBump:
    a = M(Fireworks + x);
    compare(a, 0x05);
    if (!z)
        goto InvEnemyDir;
    a = 0x00;
    writeData(TitleScreenModeValue, a);
    y = 0xfa;
    goto SetHJ;

InvEnemyDir:
    goto RXSpd;
    // ;--------------------------------
    // ;$00 - used to hold horizontal difference between player and enemy

PlayerEnemyDiff:
    a = M(Enemy_X_Position + x);
    c = 1;
    a -= M(Player_X_Position);
    writeData(TitleScreenModeValue, a);
    a = M(Enemy_PageLoc + x);
    a -= M(Player_PageLoc);
    goto Return;
    // ;--------------------------------

EnemyLanding:
    JSR(InitVStf, 482);
    a = M(Enemy_Y_Position + x);
    a &= 0xf0;
    a |= 0x08;
    writeData(Enemy_Y_Position + x, a);
    goto Return;

SubtEnemyYPos:
    a = M(Enemy_Y_Position + x);
    c = 0;
    a += 0x3e;
    compare(a, 0x44);
    goto Return;

EnemyJump:
    JSR(SubtEnemyYPos, 483);
    if (!c)
        goto DoSide;
    a = M(ExplosionTimerCounter + x);
    c = 0;
    a += 0x02;
    compare(a, 0x03);
    if (!c)
        goto DoSide;
    JSR(ChkUnderEnemy, 484);
    if (z)
        goto DoSide;
    JSR(ChkForNonSolids, 485);
    if (z)
        goto DoSide;
    JSR(EnemyLanding, 486);
    a = 0xfd;
    writeData(ExplosionTimerCounter + x, a);

DoSide:
    goto DoEnemySideCheck;
    // ;--------------------------------

HammerBroBGColl:
    JSR(ChkUnderEnemy, 487);
    if (z)
        goto NoUnderHammerBro;
    compare(a, 0x23);
    if (!z)
        goto UnderHammerBro;

KillEnemyAboveBlock:
    JSR(ShellOrBlockDefeat, 488);
    a = 0xfc;
    writeData(ExplosionTimerCounter + x, a);
    goto Return;

UnderHammerBro:
    a = M(EnemyFrameTimer + x);
    if (!z)
        goto NoUnderHammerBro;
    a = M(Enemy_State + x);
    a &= 0x88;
    writeData(Enemy_State + x, a);
    JSR(EnemyLanding, 489);
    goto DoEnemySideCheck;

NoUnderHammerBro:
    a = M(Enemy_State + x);
    a |= 0x01;
    writeData(Enemy_State + x, a);
    goto Return;

ChkUnderEnemy:
    a = 0x00;
    y = 0x15;
    goto BlockBufferChk_Enemy;

ChkForNonSolids:
    compare(a, 0x26);
    if (z)
        goto NSFnd;
    compare(a, 0xc2);
    if (z)
        goto NSFnd;
    compare(a, 0xc3);
    if (z)
        goto NSFnd;
    compare(a, 0x5f);
    if (z)
        goto NSFnd;
    compare(a, 0x60);

NSFnd:
    goto Return;
    // ;-------------------------------------------------------------------------------------

FireballBGCollision:
    a = M(Fireball_Y_Position + x);
    compare(a, 0x18);
    if (!c)
        goto ClearBounceFlag;
    JSR(BlockBufferChk_FBall, 490);
    if (z)
        goto ClearBounceFlag;
    JSR(ChkForNonSolids, 491);
    if (z)
        goto ClearBounceFlag;
    a = M(Fireball_Y_Speed + x);
    if (n)
        goto InitFireballExplode;
    a = M(FireballBouncingFlag + x);
    if (!z)
        goto InitFireballExplode;
    a = 0xfd;
    writeData(Fireball_Y_Speed + x, a);
    a = 0x01;
    writeData(FireballBouncingFlag + x, a);
    a = M(Fireball_Y_Position + x);
    a &= 0xf8;
    writeData(Fireball_Y_Position + x, a);
    goto Return;

ClearBounceFlag:
    a = 0x00;
    writeData(FireballBouncingFlag + x, a);
    goto Return;

InitFireballExplode:
    a = 0x80;
    writeData(Fireball_State + x, a);
    a = 0x02;
    writeData(Square1SoundQueue, a);
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$00 - used to hold one of bitmasks, or offset
    // ;$01 - used for relative X coordinate, also used to store middle screen page location
    // ;$02 - used for relative Y coordinate, also used to store middle screen coordinate
    // ;this data added to relative coordinates of sprite objects
    // ;stored in order: left edge, top edge, right edge, bottom edge

GetFireballBoundBox:
    a = x;
    c = 0;
    a += 0x07;
    x = a;
    y = 0x02;
    if (!z)
        goto FBallB;

GetMiscBoundBox:
    a = x;
    c = 0;
    a += 0x09;
    x = a;
    y = 0x06;

FBallB:
    JSR(BoundingBoxCore, 492);
    goto CheckRightScreenBBox;

GetEnemyBoundBox:
    y = 0x48;
    writeData(TitleScreenModeValue, y);
    y = 0x44;
    goto GetMaskedOffScrBits;

SmallPlatformBoundBox:
    y = 0x08;
    writeData(TitleScreenModeValue, y);
    y = 0x04;

GetMaskedOffScrBits:
    a = M(Enemy_X_Position + x);
    c = 1;
    a -= M(ScreenLeft_X_Pos);
    writeData(GameModeValue, a);
    a = M(Enemy_PageLoc + x);
    a -= M(ScreenLeft_PageLoc);
    if (n)
        goto CMBits;
    a |= M(GameModeValue);
    if (z)
        goto CMBits;
    y = M(TitleScreenModeValue);

CMBits:
    a = y;
    a &= M(Enemy_OffscreenBits);
    writeData(EnemyOffscrBitsMasked + x, a);
    if (!z)
        goto MoveBoundBoxOffscreen;
    goto SetupEOffsetFBBox;

LargePlatformBoundBox:
    ++x;
    JSR(GetXOffscreenBits, 493);
    --x;
    compare(a, 0xfe);
    if (c)
        goto MoveBoundBoxOffscreen;

SetupEOffsetFBBox:
    a = x;
    c = 0;
    a += 0x01;
    x = a;
    y = 0x01;
    JSR(BoundingBoxCore, 494);
    goto CheckRightScreenBBox;

MoveBoundBoxOffscreen:
    a = x;
    a <<= 1;
    a <<= 1;
    y = a;
    a = 0xff;
    writeData(EnemyBoundingBoxCoord + y, a);
    writeData(0x04b1 + y, a);
    writeData(0x04b2 + y, a);
    writeData(0x04b3 + y, a);
    goto Return;

BoundingBoxCore:
    writeData(TitleScreenModeValue, x);
    a = M(Player_Rel_YPos + y);
    writeData(VictoryModeValue, a);
    a = M(Player_Rel_XPos + y);
    writeData(GameModeValue, a);
    a = x;
    a <<= 1;
    a <<= 1;
    pha();
    y = a;
    a = M(Player_BoundBoxCtrl + x);
    a <<= 1;
    a <<= 1;
    x = a;
    a = M(GameModeValue);
    c = 0;
    a += M(BoundBoxCtrlData + x);
    writeData(BoundingBox_UL_Corner + y, a);
    a = M(GameModeValue);
    c = 0;
    a += M(0xe1ff + x);
    writeData(BoundingBox_LR_Corner + y, a);
    ++x;
    ++y;
    a = M(VictoryModeValue);
    c = 0;
    a += M(BoundBoxCtrlData + x);
    writeData(BoundingBox_UL_Corner + y, a);
    a = M(VictoryModeValue);
    c = 0;
    a += M(0xe1ff + x);
    writeData(BoundingBox_LR_Corner + y, a);
    pla();
    y = a;
    x = M(TitleScreenModeValue);
    goto Return;

CheckRightScreenBBox:
    a = M(ScreenLeft_X_Pos);
    c = 0;
    a += 0x80;
    writeData(VictoryModeValue, a);
    a = M(ScreenLeft_PageLoc);
    a += 0x00;
    writeData(GameModeValue, a);
    a = M(Player_X_Position + x);
    compare(a, M(VictoryModeValue));
    a = M(Player_PageLoc + x);
    a -= M(GameModeValue);
    if (!c)
        goto CheckLeftScreenBBox;
    a = M(BoundingBox_LR_Corner + y);
    if (n)
        goto NoOfs;
    a = 0xff;
    x = M(BoundingBox_UL_Corner + y);
    if (n)
        goto SORte;
    writeData(BoundingBox_UL_Corner + y, a);

SORte:
    writeData(BoundingBox_LR_Corner + y, a);

NoOfs:
    x = M(BulletBill_FrenzyVar);
    goto Return;

CheckLeftScreenBBox:
    a = M(BoundingBox_UL_Corner + y);
    if (!n)
        goto NoOfs2;
    compare(a, 0xa0);
    if (!c)
        goto NoOfs2;
    a = 0x00;
    x = M(BoundingBox_LR_Corner + y);
    if (!n)
        goto SOLft;
    writeData(BoundingBox_LR_Corner + y, a);

SOLft:
    writeData(BoundingBox_UL_Corner + y, a);

NoOfs2:
    x = M(BulletBill_FrenzyVar);
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$06 - second object's offset
    // ;$07 - counter

PlayerCollisionCore:
    x = 0x00;

SprObjectCollisionCore:
    writeData(World7, y);
    a = 0x01;
    writeData(World8, a);

CollisionCoreLoop:
    a = M(BoundingBox_UL_Corner + y);
    compare(a, M(BoundingBox_UL_Corner + x));
    if (c)
        goto FirstBoxGreater;
    compare(a, M(BoundingBox_LR_Corner + x));
    if (!c)
        goto SecondBoxVerticalChk;
    if (z)
        goto CollisionFound;
    a = M(BoundingBox_LR_Corner + y);
    compare(a, M(BoundingBox_UL_Corner + y));
    if (!c)
        goto CollisionFound;
    compare(a, M(BoundingBox_UL_Corner + x));
    if (c)
        goto CollisionFound;
    y = M(World7);
    goto Return;

SecondBoxVerticalChk:
    a = M(BoundingBox_LR_Corner + x);
    compare(a, M(BoundingBox_UL_Corner + x));
    if (!c)
        goto CollisionFound;
    a = M(BoundingBox_LR_Corner + y);
    compare(a, M(BoundingBox_UL_Corner + x));
    if (c)
        goto CollisionFound;
    y = M(World7);
    goto Return;

FirstBoxGreater:
    compare(a, M(BoundingBox_UL_Corner + x));
    if (z)
        goto CollisionFound;
    compare(a, M(BoundingBox_LR_Corner + x));
    if (!c)
        goto CollisionFound;
    if (z)
        goto CollisionFound;
    compare(a, M(BoundingBox_LR_Corner + y));
    if (!c)
        goto NoCollisionFound;
    if (z)
        goto NoCollisionFound;
    a = M(BoundingBox_LR_Corner + y);
    compare(a, M(BoundingBox_UL_Corner + x));
    if (c)
        goto CollisionFound;

NoCollisionFound:
    c = 0;
    y = M(World7);
    goto Return;

CollisionFound:
    ++x;
    ++y;
    --M(World8);
    if (!n)
        goto CollisionCoreLoop;
    c = 1;
    y = M(World7);
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$02 - modified y coordinate
    // ;$03 - stores metatile involved in block buffer collisions
    // ;$04 - comes in with offset to block buffer adder data, goes out with low nybble x/y coordinate
    // ;$05 - modified x coordinate
    // ;$06-$07 - block buffer address

BlockBufferChk_Enemy:
    pha();
    a = x;
    c = 0;
    a += 0x01;
    x = a;
    pla();
    goto BBChk_E;

ResidualMiscObjectCode:
    a = x;
    c = 0;
    a += 0x0d;
    x = a;
    y = 0x1b;
    goto ResJmpM;

BlockBufferChk_FBall:
    y = 0x1a;
    a = x;
    c = 0;
    a += 0x07;
    x = a;

ResJmpM:
    a = 0x00;

BBChk_E:
    JSR(BlockBufferCollision, 495);
    x = M(BulletBill_FrenzyVar);
    compare(a, 0x00);
    goto Return;

BlockBufferColli_Feet:
    ++y;

BlockBufferColli_Head:
    a = 0x00;
    bit(M(0x01a9));
    goto loc_E3EE;

BlockBufferColli_Side:
    a = 0x01;
loc_E3EE:
    x = 0x00;

BlockBufferCollision:
    pha();
    writeData(World5, y);
    a = M(BlockBuffer_X_Adder + y);
    c = 0;
    a += M(Player_X_Position + x);
    writeData(World6, a);
    a = M(Player_PageLoc + x);
    a += 0x00;
    a &= 0x01;
    a >>= 1;
    a |= M(World6);
    a.ror();
    a >>= 1;
    a >>= 1;
    a >>= 1;
    JSR(GetBlockBufferAddr, 496);
    y = M(World5);
    a = M(Player_Y_Position + x);
    c = 0;
    a += M(BlockBuffer_Y_Adder + y);
    a &= 0xf0;
    c = 1;
    a -= 0x20;
    writeData(VictoryModeValue, a);
    y = a;
    a = M(W(World7) + y);
    writeData(GameOverModeValue, a);
    y = M(World5);
    pla();
    if (!z)
        goto RetXC;
    a = M(Player_Y_Position + x);
    goto RetYC;

RetXC:
    a = M(Player_X_Position + x);

RetYC:
    a &= 0x0f;
    writeData(World5, a);
    a = M(GameOverModeValue);
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;unused byte
    // ;-------------------------------------------------------------------------------------
    // ;$00 - offset to vine Y coordinate adder
    // ;$02 - offset to sprite data

DrawVine:
    writeData(TitleScreenModeValue, y);
    a = M(Enemy_Rel_YPos);
    c = 0;
    a += M(VineYPosAdder + y);
    x = M(VineObjOffset + y);
    y = M(Enemy_SprDataOffset + x);
    writeData(VictoryModeValue, y);
    JSR(SixSpriteStacker, 497);
    a = M(Enemy_Rel_XPos);
    writeData(Sprite_X_Position + y, a);
    writeData(0x020b + y, a);
    writeData(0x0213 + y, a);
    c = 0;
    a += 0x06;
    writeData(0x0207 + y, a);
    writeData(0x020f + y, a);
    writeData(0x0217 + y, a);
    a = 0x21;
    writeData(Sprite_Attributes + y, a);
    writeData(0x020a + y, a);
    writeData(0x0212 + y, a);
    a |= 0x40;
    writeData(0x0206 + y, a);
    writeData(0x020e + y, a);
    writeData(0x0216 + y, a);
    x = 0x05;

VineTL:
    a = 0xe1;
    writeData(Sprite_Tilenumber + y, a);
    ++y;
    ++y;
    ++y;
    ++y;
    --x;
    if (!n)
        goto VineTL;
    y = M(VictoryModeValue);
    a = M(TitleScreenModeValue);
    if (!z)
        goto SkpVTop;
    a = 0xe0;
    writeData(Sprite_Tilenumber + y, a);

SkpVTop:
    x = 0x00;

ChkFTop:
    a = M(VineStart_Y_Position);
    c = 1;
    a -= M(Sprite_Y_Position + y);
    compare(a, 0x64);
    if (!c)
        goto NextVSp;
    a = 0xf8;
    writeData(Sprite_Y_Position + y, a);

NextVSp:
    ++y;
    ++y;
    ++y;
    ++y;
    ++x;
    compare(x, 0x06);
    if (!z)
        goto ChkFTop;
    y = M(TitleScreenModeValue);
    goto Return;

SixSpriteStacker:
    x = 0x06;

StkLp:
    writeData(Sprite_Y_Position + y, a);
    c = 0;
    a += 0x08;
    ++y;
    ++y;
    ++y;
    ++y;
    --x;
    if (!z)
        goto StkLp;
    y = M(VictoryModeValue);
    goto Return;
    // ;-------------------------------------------------------------------------------------

DrawHammer:
    y = M(Misc_SprDataOffset + x);
    a = M(TimerControl);
    if (!z)
        goto ForceHPose;
    a = M(Misc_State + x);
    a &= 0x7f;
    compare(a, 0x01);
    if (z)
        goto GetHPose;

ForceHPose:
    x = 0x00;
    if (z)
        goto RenderH;

GetHPose:
    a = M(TallEnemy);
    a >>= 1;
    a >>= 1;
    a &= 0x03;
    x = a;

RenderH:
    a = M(Misc_Rel_YPos);
    c = 0;
    a += M(FirstSprYPos + x);
    writeData(Sprite_Y_Position + y, a);
    c = 0;
    a += M(SecondSprYPos + x);
    writeData(0x0204 + y, a);
    a = M(Misc_Rel_XPos);
    c = 0;
    a += M(FirstSprXPos + x);
    writeData(Sprite_X_Position + y, a);
    c = 0;
    a += M(SecondSprXPos + x);
    writeData(0x0207 + y, a);
    a = M(FirstSprTilenum + x);
    writeData(Sprite_Tilenumber + y, a);
    a = M(SecondSprTilenum + x);
    writeData(0x0205 + y, a);
    a = M(HammerSprAttrib + x);
    writeData(Sprite_Attributes + y, a);
    writeData(0x0206 + y, a);
    x = M(BulletBill_FrenzyVar);
    a = M(Misc_OffscreenBits);
    a &= 0xfc;
    if (z)
        goto NoHOffscr;
    a = 0x00;
    writeData(Misc_State + x, a);
    a = 0xf8;
    JSR(DumpTwoSpr, 498);

NoHOffscr:
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$00-$01 - used to hold tile numbers ($01 addressed in draw floatey number part)
    // ;$02 - used to hold Y coordinate for floatey number
    // ;$03 - residual byte used for flip (but value set here affects nothing)
    // ;$04 - attribute byte for floatey number
    // ;$05 - used as X coordinate for floatey number

FlagpoleGfxHandler:
    y = M(Enemy_SprDataOffset + x);
    a = M(Enemy_Rel_XPos);
    writeData(Sprite_X_Position + y, a);
    c = 0;
    a += 0x08;
    writeData(0x0207 + y, a);
    writeData(0x020b + y, a);
    c = 0;
    a += 0x0c;
    writeData(World6, a);
    a = M(Enemy_Y_Position + x);
    JSR(DumpTwoSpr, 499);
    a += 0x08;
    writeData(0x0208 + y, a);
    a = M(FlagpoleFNum_Y_Pos);
    writeData(VictoryModeValue, a);
    a = 0x01;
    writeData(GameOverModeValue, a);
    writeData(World5, a);
    writeData(Sprite_Attributes + y, a);
    writeData(0x0206 + y, a);
    writeData(0x020a + y, a);
    a = 0x7e;
    writeData(Sprite_Tilenumber + y, a);
    writeData(0x0209 + y, a);
    a = 0x7f;
    writeData(0x0205 + y, a);
    a = M(FlagpoleCollisionYPos);
    if (z)
        goto ChkFlagOffscreen;
    a = y;
    c = 0;
    a += 0x0c;
    y = a;
    a = M(FlagpoleScore);
    a <<= 1;
    x = a;
    a = M(FlagpoleScoreNumTiles + x);
    writeData(TitleScreenModeValue, a);
    a = M(0xe542 + x);
    JSR(DrawOneSpriteRow, 500);

ChkFlagOffscreen:
    x = M(BulletBill_FrenzyVar);
    y = M(Enemy_SprDataOffset + x);
    a = M(Enemy_OffscreenBits);
    a &= 0x0e;
    if (z)
        goto ExitDumpSpr;
    // ;-------------------------------------------------------------------------------------

MoveSixSpritesOffscreen:
    a = 0xf8;

DumpSixSpr:
    writeData(0x0214 + y, a);
    writeData(0x0210 + y, a);

DumpFourSpr:
    writeData(0x020c + y, a);

DumpThreeSpr:
    writeData(0x0208 + y, a);

DumpTwoSpr:
    writeData(0x0204 + y, a);
    writeData(Sprite_Y_Position + y, a);

ExitDumpSpr:
    goto Return;
    // ;-------------------------------------------------------------------------------------

DrawLargePlatform:
    y = M(Enemy_SprDataOffset + x);
    writeData(VictoryModeValue, y);
    ++y;
    ++y;
    ++y;
    a = M(Enemy_Rel_XPos);
    JSR(SixSpriteStacker, 501);
    x = M(BulletBill_FrenzyVar);
    a = M(Enemy_Y_Position + x);
    JSR(DumpFourSpr, 502);
    y = M(AreaType);
    compare(y, 0x03);
    if (z)
        goto ShrinkPlatform;
    y = M(SecondaryHardMode);
    if (z)
        goto SetLast2Platform;

ShrinkPlatform:
    a = 0xf8;

SetLast2Platform:
    y = M(Enemy_SprDataOffset + x);
    writeData(0x0210 + y, a);
    writeData(0x0214 + y, a);
    a = 0x5b;
    x = M(CloudTypeOverride);
    if (z)
        goto SetPlatformTilenum;
    a = 0x75;

SetPlatformTilenum:
    x = M(BulletBill_FrenzyVar);
    ++y;
    JSR(DumpSixSpr, 503);
    a = 0x02;
    ++y;
    JSR(DumpSixSpr, 504);
    ++x;
    JSR(GetXOffscreenBits, 505);
    --x;
    y = M(Enemy_SprDataOffset + x);
    a <<= 1;
    pha();
    if (!c)
        goto SChk2;
    a = 0xf8;
    writeData(Sprite_Y_Position + y, a);

SChk2:
    pla();
    a <<= 1;
    pha();
    if (!c)
        goto SChk3;
    a = 0xf8;
    writeData(0x0204 + y, a);

SChk3:
    pla();
    a <<= 1;
    pha();
    if (!c)
        goto SChk4;
    a = 0xf8;
    writeData(0x0208 + y, a);

SChk4:
    pla();
    a <<= 1;
    pha();
    if (!c)
        goto SChk5;
    a = 0xf8;
    writeData(0x020c + y, a);

SChk5:
    pla();
    a <<= 1;
    pha();
    if (!c)
        goto SChk6;
    a = 0xf8;
    writeData(0x0210 + y, a);

SChk6:
    pla();
    a <<= 1;
    if (!c)
        goto SLChk;
    a = 0xf8;
    writeData(0x0214 + y, a);

SLChk:
    a = M(Enemy_OffscreenBits);
    a <<= 1;
    if (!c)
        goto ExDLPl;
    JSR(MoveSixSpritesOffscreen, 506);

ExDLPl:
    goto Return;
    // ;-------------------------------------------------------------------------------------

DrawFloateyNumber_Coin:
    a = M(TallEnemy);
    a >>= 1;
    if (c)
        goto NotRsNum;
    --M(Misc_Y_Position + x);

NotRsNum:
    a = M(Misc_Y_Position + x);
    JSR(DumpTwoSpr, 507);
    a = M(Misc_Rel_XPos);
    writeData(Sprite_X_Position + y, a);
    c = 0;
    a += 0x08;
    writeData(0x0207 + y, a);
    a = 0x02;
    writeData(Sprite_Attributes + y, a);
    writeData(0x0206 + y, a);
    a = 0xf7;
    writeData(Sprite_Tilenumber + y, a);
    a = 0xfb;
    writeData(0x0205 + y, a);
    goto ExJCGfx;

JCoinGfxHandler:
    y = M(Misc_SprDataOffset + x);
    a = M(Misc_State + x);
    compare(a, 0x02);
    if (c)
        goto DrawFloateyNumber_Coin;
    a = M(Misc_Y_Position + x);
    writeData(Sprite_Y_Position + y, a);
    c = 0;
    a += 0x08;
    writeData(0x0204 + y, a);
    a = M(Misc_Rel_XPos);
    writeData(Sprite_X_Position + y, a);
    writeData(0x0207 + y, a);
    a = M(TallEnemy);
    a >>= 1;
    a &= 0x03;
    x = a;
    a = M(JumpingCoinTiles + x);
    ++y;
    JSR(DumpTwoSpr, 508);
    --y;
    a = 0x02;
    writeData(Sprite_Attributes + y, a);
    a = 0x82;
    writeData(0x0206 + y, a);
    x = M(BulletBill_FrenzyVar);

ExJCGfx:
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$00-$01 - used to hold tiles for drawing the power-up, $00 also used to hold power-up type
    // ;$02 - used to hold bottom row Y position
    // ;$03 - used to hold flip control (not used here)
    // ;$04 - used to hold sprite attributes
    // ;$05 - used to hold X position
    // ;$07 - counter
    // ;tiles arranged in top left, right, bottom left, right order

DrawPowerUp:
    y = M(0x06ea);
    a = M(Enemy_Rel_YPos);
    c = 0;
    a += 0x08;
    writeData(VictoryModeValue, a);
    a = M(Enemy_Rel_XPos);
    writeData(World6, a);
    x = M(PowerUpType);
    a = M(PowerUpAttributes + x);
    a |= M(0x03ca);
    writeData(World5, a);
    a = x;
    pha();
    a <<= 1;
    a <<= 1;
    x = a;
    a = 0x01;
    writeData(World8, a);
    writeData(GameOverModeValue, a);

PUpDrawLoop:
    a = M(PowerUpGfxTable + x);
    writeData(TitleScreenModeValue, a);
    a = M(0xe6bf + x);
    JSR(DrawOneSpriteRow, 509);
    --M(World8);
    if (!n)
        goto PUpDrawLoop;
    y = M(0x06ea);
    pla();
    if (z)
        goto PUpOfs;
    compare(a, 0x03);
    if (z)
        goto PUpOfs;
    writeData(TitleScreenModeValue, a);
    a = M(TallEnemy);
    a >>= 1;
    a &= 0x03;
    a |= M(0x03ca);
    writeData(Sprite_Attributes + y, a);
    writeData(0x0206 + y, a);
    x = M(TitleScreenModeValue);
    --x;
    if (z)
        goto FlipPUpRightSide;
    writeData(0x020a + y, a);
    writeData(0x020e + y, a);

FlipPUpRightSide:
    a = M(0x0206 + y);
    a |= 0x40;
    writeData(0x0206 + y, a);
    a = M(0x020e + y);
    a |= 0x40;
    writeData(0x020e + y, a);

PUpOfs:
    goto SprObjectOffscrChk;
    // ;-------------------------------------------------------------------------------------
    // ;$00-$01 - used in DrawEnemyObjRow to hold sprite tile numbers
    // ;$02 - used to store Y position
    // ;$03 - used to store moving direction, used to flip enemies horizontally
    // ;$04 - used to store enemy's sprite attributes
    // ;$05 - used to store X position
    // ;$eb - used to hold sprite data offset
    // ;$ec - used to hold either altered enemy state or special value used in gfx handler as condition
    // ;$ed - used to hold enemy state from buffer
    // ;$ef - used to hold enemy code used in gfx handler (may or may not resemble Enemy_ID values)
    // ;tiles arranged in top left, right, middle left, right, bottom left, right order

EnemyGfxHandler:
    a = M(Enemy_Y_Position + x);
    writeData(VictoryModeValue, a);
    a = M(Enemy_Rel_XPos);
    writeData(World6, a);
    y = M(Enemy_SprDataOffset + x);
    writeData(0x00eb, y);
    a = 0x00;
    writeData(VerticalFlipFlag, a);
    a = M(Enemy_MovingDir + x);
    writeData(GameOverModeValue, a);
    a = M(Enemy_SprAttrib + x);
    writeData(World5, a);
    a = M(Fireworks + x);
    compare(a, 0x0d);
    if (!z)
        goto CheckForRetainerObj;
    y = M(ExplosionGfxCounter + x);
    if (n)
        goto CheckForRetainerObj;
    y = M(EnemyFrameTimer + x);
    if (z)
        goto CheckForRetainerObj;
    goto Return;

CheckForRetainerObj:
    a = M(Enemy_State + x);
    writeData(0x00ed, a);
    a &= 0x1f;
    y = a;
    a = M(Fireworks + x);
    compare(a, 0x35);
    if (!z)
        goto CheckForBulletBillCV;
    y = 0x00;
    a = 0x01;
    writeData(GameOverModeValue, a);
    a = 0x15;

CheckForBulletBillCV:
    compare(a, 0x33);
    if (!z)
        goto CheckForJumpspring;
    --M(VictoryModeValue);
    a = 0x03;
    y = M(EnemyFrameTimer + x);
    if (z)
        goto SBBAt;
    a |= 0x20;

SBBAt:
    writeData(World5, a);
    y = 0x00;
    writeData(0x00ed, y);
    a = 0x08;

CheckForJumpspring:
    compare(a, 0x32);
    if (!z)
        goto CheckForPodoboo;
    y = 0x03;
    x = M(JumpspringAnimCtrl);
    a = M(JumpspringFrameOffsets + x);

CheckForPodoboo:
    writeData(0x00ef, a);
    writeData(0x00ec, y);
    x = M(BulletBill_FrenzyVar);
    compare(a, 0x0c);
    if (!z)
        goto CheckBowserGfxFlag;
    a = M(ExplosionTimerCounter + x);
    if (n)
        goto CheckBowserGfxFlag;
    ++M(VerticalFlipFlag);

CheckBowserGfxFlag:
    a = M(BowserGfxFlag);
    if (z)
        goto CheckForGoomba;
    y = 0x16;
    compare(a, 0x01);
    if (z)
        goto SBwsrGfxOfs;
    ++y;

SBwsrGfxOfs:
    writeData(0x00ef, y);

CheckForGoomba:
    y = M(0x00ef);
    compare(y, 0x06);
    if (!z)
        goto CheckBowserFront;
    a = M(Enemy_State + x);
    compare(a, 0x02);
    if (!c)
        goto GmbaAnim;
    x = 0x04;
    writeData(0x00ec, x);

GmbaAnim:
    a &= 0x20;
    a |= M(TimerControl);
    if (!z)
        goto CheckBowserFront;
    a = M(TallEnemy);
    a &= 0x08;
    if (!z)
        goto CheckBowserFront;
    a = M(GameOverModeValue);
    a ^= 0x03;
    writeData(GameOverModeValue, a);

CheckBowserFront:
    a = M(EnemyAttributeData + y);
    a |= M(World5);
    writeData(World5, a);
    a = M(EnemyGfxTableOffsets + y);
    x = a;
    y = M(0x00ec);
    a = M(BowserGfxFlag);
    if (z)
        goto CheckForSpiny;
    compare(a, 0x01);
    if (!z)
        goto CheckBowserRear;
    a = M(BowserBodyControls);
    if (!n)
        goto ChkFrontSte;
    x = 0xde;

ChkFrontSte:
    a = M(0x00ed);
    a &= 0x20;
    if (z)
        goto DrawBowser;

FlipBowserOver:
    writeData(VerticalFlipFlag, x);

DrawBowser:
    goto DrawEnemyObject;

CheckBowserRear:
    a = M(BowserBodyControls);
    a &= 0x01;
    if (z)
        goto ChkRearSte;
    x = 0xe4;

ChkRearSte:
    a = M(0x00ed);
    a &= 0x20;
    if (z)
        goto DrawBowser;
    a = M(VictoryModeValue);
    c = 1;
    a -= 0x10;
    writeData(VictoryModeValue, a);
    goto FlipBowserOver;

CheckForSpiny:
    compare(x, 0x24);
    if (!z)
        goto CheckForLakitu;
    compare(y, 0x05);
    if (!z)
        goto NotEgg;
    x = 0x30;
    a = 0x02;
    writeData(GameOverModeValue, a);
    a = 0x05;
    writeData(0x00ec, a);

NotEgg:
    goto CheckForHammerBro;

CheckForLakitu:
    compare(x, 0x90);
    if (!z)
        goto CheckUpsideDownShell;
    a = M(0x00ed);
    a &= 0x20;
    if (!z)
        goto NoLAFr;
    a = M(FrenzyEnemyTimer);
    compare(a, 0x10);
    if (c)
        goto NoLAFr;
    x = 0x96;

NoLAFr:
    goto CheckDefeatedState;

CheckUpsideDownShell:
    a = M(0x00ef);
    compare(a, 0x04);
    if (c)
        goto CheckRightSideUpShell;
    compare(y, 0x02);
    if (!c)
        goto CheckRightSideUpShell;
    x = 0x5a;
    y = M(0x00ef);
    compare(y, 0x02);
    if (!z)
        goto CheckRightSideUpShell;
    x = 0x7e;
    ++M(VictoryModeValue);

CheckRightSideUpShell:
    a = M(0x00ec);
    compare(a, 0x04);
    if (!z)
        goto CheckForHammerBro;
    x = 0x72;
    ++M(VictoryModeValue);
    y = M(0x00ef);
    compare(y, 0x02);
    if (z)
        goto CheckForDefdGoomba;
    x = 0x66;
    ++M(VictoryModeValue);

CheckForDefdGoomba:
    compare(y, 0x06);
    if (!z)
        goto CheckForHammerBro;
    x = 0x54;
    a = M(0x00ed);
    a &= 0x20;
    if (!z)
        goto CheckForHammerBro;
    x = 0x8a;
    --M(VictoryModeValue);

CheckForHammerBro:
    y = M(BulletBill_FrenzyVar);
    a = M(0x00ef);
    compare(a, 0x05);
    if (!z)
        goto CheckForBloober;
    a = M(0x00ed);
    if (z)
        goto CheckToAnimateEnemy;
    a &= 0x08;
    if (z)
        goto CheckDefeatedState;
    x = 0xb4;
    if (!z)
        goto CheckToAnimateEnemy;

CheckForBloober:
    compare(x, 0x48);
    if (z)
        goto CheckToAnimateEnemy;
    a = M(EnemyIntervalTimer + y);
    compare(a, 0x05);
    if (c)
        goto CheckDefeatedState;
    compare(x, 0x3c);
    if (!z)
        goto CheckToAnimateEnemy;
    compare(a, 0x01);
    if (z)
        goto CheckDefeatedState;
    ++M(VictoryModeValue);
    ++M(VictoryModeValue);
    ++M(VictoryModeValue);
    goto CheckAnimationStop;

CheckToAnimateEnemy:
    a = M(0x00ef);
    compare(a, 0x06);
    if (z)
        goto CheckDefeatedState;
    compare(a, 0x08);
    if (z)
        goto CheckDefeatedState;
    compare(a, 0x0c);
    if (z)
        goto CheckDefeatedState;
    compare(a, 0x18);
    if (c)
        goto CheckDefeatedState;
    y = 0x00;
    compare(a, 0x15);
    if (!z)
        goto CheckForSecondFrame;
    ++y;
    a = M(WorldNumber);
    compare(a, 0x07);
    if (c)
        goto CheckDefeatedState;
    x = 0xa2;
    a = 0x03;
    writeData(0x00ec, a);
    if (!z)
        goto CheckDefeatedState;

CheckForSecondFrame:
    a = M(TallEnemy);
    a &= M(EnemyAnimTimingBMask + y);
    if (!z)
        goto CheckDefeatedState;

CheckAnimationStop:
    a = M(0x00ed);
    a &= 0xa0;
    a |= M(TimerControl);
    if (!z)
        goto CheckDefeatedState;
    a = x;
    c = 0;
    a += 0x06;
    x = a;

CheckDefeatedState:
    a = M(0x00ed);
    a &= 0x20;
    if (z)
        goto DrawEnemyObject;
    a = M(0x00ef);
    compare(a, 0x04);
    if (!c)
        goto DrawEnemyObject;
    y = 0x01;
    writeData(VerticalFlipFlag, y);
    --y;
    writeData(0x00ec, y);

DrawEnemyObject:
    y = M(0x00eb);
    JSR(DrawEnemyObjRow, 510);
    JSR(DrawEnemyObjRow, 511);
    JSR(DrawEnemyObjRow, 512);
    x = M(BulletBill_FrenzyVar);
    y = M(Enemy_SprDataOffset + x);
    a = M(0x00ef);
    compare(a, 0x08);
    if (!z)
        goto CheckForVerticalFlip;

SkipToOffScrChk:
    goto SprObjectOffscrChk;

CheckForVerticalFlip:
    a = M(VerticalFlipFlag);
    if (z)
        goto CheckForESymmetry;
    a = M(Sprite_Attributes + y);
    a |= 0x80;
    ++y;
    ++y;
    JSR(DumpSixSpr, 513);
    --y;
    --y;
    a = y;
    x = a;
    a = M(0x00ef);
    compare(a, 0x05);
    if (z)
        goto FlipEnemyVertically;
    compare(a, 0x11);
    if (z)
        goto FlipEnemyVertically;
    compare(a, 0x15);
    if (c)
        goto FlipEnemyVertically;
    a = x;
    c = 0;
    a += 0x08;
    x = a;

FlipEnemyVertically:
    a = M(Sprite_Tilenumber + x);
    pha();
    a = M(0x0205 + x);
    pha();
    a = M(0x0211 + y);
    writeData(Sprite_Tilenumber + x, a);
    a = M(0x0215 + y);
    writeData(0x0205 + x, a);
    pla();
    writeData(0x0215 + y, a);
    pla();
    writeData(0x0211 + y, a);

CheckForESymmetry:
    a = M(BowserGfxFlag);
    if (!z)
        goto SkipToOffScrChk;
    a = M(0x00ef);
    x = M(0x00ec);
    compare(a, 0x05);
    if (!z)
        goto ContES;
    goto SprObjectOffscrChk;

ContES:
    compare(a, 0x07);
    if (z)
        goto MirrorEnemyGfx;
    compare(a, 0x0d);
    if (z)
        goto MirrorEnemyGfx;
    compare(a, 0x0c);
    if (z)
        goto MirrorEnemyGfx;
    compare(a, 0x12);
    if (!z)
        goto ESRtnr;
    compare(x, 0x05);
    if (!z)
        goto CheckToMirrorLakitu;

ESRtnr:
    compare(a, 0x15);
    if (!z)
        goto SpnySC;
    a = 0x42;
    writeData(0x0216 + y, a);

SpnySC:
    compare(x, 0x02);
    if (!c)
        goto CheckToMirrorLakitu;

MirrorEnemyGfx:
    a = M(BowserGfxFlag);
    if (!z)
        goto CheckToMirrorLakitu;
    a = M(Sprite_Attributes + y);
    a &= 0xa3;
    writeData(Sprite_Attributes + y, a);
    writeData(0x020a + y, a);
    writeData(0x0212 + y, a);
    a |= 0x40;
    compare(x, 0x05);
    if (!z)
        goto EggExc;
    a |= 0x80;

EggExc:
    writeData(0x0206 + y, a);
    writeData(0x020e + y, a);
    writeData(0x0216 + y, a);
    compare(x, 0x04);
    if (!z)
        goto CheckToMirrorLakitu;
    a = M(0x020a + y);
    a |= 0x80;
    writeData(0x020a + y, a);
    writeData(0x0212 + y, a);
    a |= 0x40;
    writeData(0x020e + y, a);
    writeData(0x0216 + y, a);

CheckToMirrorLakitu:
    a = M(0x00ef);
    compare(a, 0x11);
    if (!z)
        goto CheckToMirrorJSpring;
    a = M(VerticalFlipFlag);
    if (!z)
        goto NVFLak;
    a = M(0x0212 + y);
    a &= 0x81;
    writeData(0x0212 + y, a);
    a = M(0x0216 + y);
    a |= 0x41;
    writeData(0x0216 + y, a);
    x = M(FrenzyEnemyTimer);
    compare(x, 0x10);
    if (c)
        goto SprObjectOffscrChk;
    writeData(0x020e + y, a);
    a &= 0x81;
    writeData(0x020a + y, a);
    if (!c)
        goto SprObjectOffscrChk;

NVFLak:
    a = M(Sprite_Attributes + y);
    a &= 0x81;
    writeData(Sprite_Attributes + y, a);
    a = M(0x0206 + y);
    a |= 0x41;
    writeData(0x0206 + y, a);

CheckToMirrorJSpring:
    a = M(0x00ef);
    compare(a, 0x18);
    if (!c)
        goto SprObjectOffscrChk;
    a = 0x82;
    writeData(0x020a + y, a);
    writeData(0x0212 + y, a);
    a |= 0x40;
    writeData(0x020e + y, a);
    writeData(0x0216 + y, a);

SprObjectOffscrChk:
    x = M(BulletBill_FrenzyVar);
    a = M(Enemy_OffscreenBits);
    a >>= 1;
    a >>= 1;
    a >>= 1;
    pha();
    if (!c)
        goto LcChk;
    a = 0x04;
    JSR(MoveESprColOffscreen, 514);

LcChk:
    pla();
    a >>= 1;
    pha();
    if (!c)
        goto Row3C;
    a = 0x00;
    JSR(MoveESprColOffscreen, 515);

Row3C:
    pla();
    a >>= 1;
    a >>= 1;
    pha();
    if (!c)
        goto Row23C;
    a = 0x10;
    JSR(MoveESprRowOffscreen, 516);

Row23C:
    pla();
    a >>= 1;
    pha();
    if (!c)
        goto AllRowC;
    a = 0x08;
    JSR(MoveESprRowOffscreen, 517);

AllRowC:
    pla();
    a >>= 1;
    if (!c)
        goto ExEGHandler;
    JSR(MoveESprRowOffscreen, 518);
    a = M(Fireworks + x);
    compare(a, 0x0c);
    if (z)
        goto ExEGHandler;
    a = M(Enemy_Y_HighPos + x);
    compare(a, 0x02);
    if (!z)
        goto ExEGHandler;
    JSR(EraseEnemyObject, 519);

ExEGHandler:
    goto Return;

DrawEnemyObjRow:
    a = M(EnemyGraphicsTable + x);
    writeData(TitleScreenModeValue, a);
    a = M(0xe73f + x);

DrawOneSpriteRow:
    writeData(GameModeValue, a);
    goto DrawSpriteObject;

MoveESprRowOffscreen:
    c = 0;
    a += M(Enemy_SprDataOffset + x);
    y = a;
    a = 0xf8;
    goto DumpTwoSpr;

MoveESprColOffscreen:
    c = 0;
    a += M(Enemy_SprDataOffset + x);
    y = a;
    JSR(MoveColOffscreen, 520);
    writeData(0x0210 + y, a);
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$00-$01 - tile numbers
    // ;$02 - relative Y position
    // ;$03 - horizontal flip flag (not used here)
    // ;$04 - attributes
    // ;$05 - relative X position

DrawBlock:
    a = M(Block_Rel_YPos);
    writeData(VictoryModeValue, a);
    a = M(Block_Rel_XPos);
    writeData(World6, a);
    a = 0x03;
    writeData(World5, a);
    a >>= 1;
    writeData(GameOverModeValue, a);
    y = M(Alt_SprDataOffset + x);
    x = 0x00;

DBlkLoop:
    a = M(DefaultBlockObjTiles + x);
    writeData(TitleScreenModeValue, a);
    a = M(0xebce + x);
    JSR(DrawOneSpriteRow, 521);
    compare(x, 0x04);
    if (!z)
        goto DBlkLoop;
    x = M(BulletBill_FrenzyVar);
    y = M(Alt_SprDataOffset + x);
    a = M(AreaType);
    compare(a, 0x01);
    if (z)
        goto ChkRep;
    a = 0x86;
    writeData(Sprite_Tilenumber + y, a);
    writeData(0x0205 + y, a);

ChkRep:
    a = M(Block_Metatile + x);
    compare(a, 0xc4);
    if (!z)
        goto BlkOffscr;
    a = 0x87;
    ++y;
    JSR(DumpFourSpr, 522);
    --y;
    a = 0x03;
    x = M(AreaType);
    --x;
    if (z)
        goto SetBFlip;
    a >>= 1;

SetBFlip:
    x = M(BulletBill_FrenzyVar);
    writeData(Sprite_Attributes + y, a);
    a |= 0x40;
    writeData(0x0206 + y, a);
    a |= 0x80;
    writeData(0x020e + y, a);
    a &= 0x83;
    writeData(0x020a + y, a);

BlkOffscr:
    a = M(Block_OffscreenBits);
    pha();
    a &= 0x04;
    if (z)
        goto PullOfsB;
    a = 0xf8;
    writeData(0x0204 + y, a);
    writeData(0x020c + y, a);

PullOfsB:
    pla();

ChkLeftCo:
    a &= 0x08;
    if (z)
        goto ExDBlk;

MoveColOffscreen:
    a = 0xf8;
    writeData(Sprite_Y_Position + y, a);
    writeData(0x0208 + y, a);

ExDBlk:
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$00 - used to hold palette bits for attribute byte or relative X position

DrawBrickChunks:
    a = 0x02;
    writeData(TitleScreenModeValue, a);
    a = 0x75;
    y = M(GreenParatroopaJump);
    compare(y, 0x05);
    if (z)
        goto DChunks;
    a = 0x03;
    writeData(TitleScreenModeValue, a);
    a = 0x84;

DChunks:
    y = M(Alt_SprDataOffset + x);
    ++y;
    JSR(DumpFourSpr, 523);
    a = M(TallEnemy);
    a <<= 1;
    a <<= 1;
    a <<= 1;
    a <<= 1;
    a &= 0xc0;
    a |= M(TitleScreenModeValue);
    ++y;
    JSR(DumpFourSpr, 524);
    --y;
    --y;
    a = M(Block_Rel_YPos);
    JSR(DumpTwoSpr, 525);
    a = M(Block_Rel_XPos);
    writeData(Sprite_X_Position + y, a);
    a = M(Block_Orig_XPos + x);
    c = 1;
    a -= M(ScreenLeft_X_Pos);
    writeData(TitleScreenModeValue, a);
    c = 1;
    a -= M(Block_Rel_XPos);
    a += M(TitleScreenModeValue);
    a += 0x06;
    writeData(0x0207 + y, a);
    a = M(0x03bd);
    writeData(0x0208 + y, a);
    writeData(0x020c + y, a);
    a = M(0x03b2);
    writeData(0x020b + y, a);
    a = M(TitleScreenModeValue);
    c = 1;
    a -= M(0x03b2);
    a += M(TitleScreenModeValue);
    a += 0x06;
    writeData(0x020f + y, a);
    a = M(Block_OffscreenBits);
    JSR(ChkLeftCo, 526);
    a = M(Block_OffscreenBits);
    a <<= 1;
    if (!c)
        goto ChnkOfs;
    a = 0xf8;
    JSR(DumpTwoSpr, 527);

ChnkOfs:
    a = M(TitleScreenModeValue);
    if (!n)
        goto ExBCDr;
    a = M(Sprite_X_Position + y);
    compare(a, M(0x0207 + y));
    if (!c)
        goto ExBCDr;
    a = 0xf8;
    writeData(0x0204 + y, a);
    writeData(0x020c + y, a);

ExBCDr:
    goto Return;
    // ;-------------------------------------------------------------------------------------

DrawFireball:
    y = M(FBall_SprDataOffset + x);
    a = M(Fireball_Rel_YPos);
    writeData(Sprite_Y_Position + y, a);
    a = M(Fireball_Rel_XPos);
    writeData(Sprite_X_Position + y, a);

DrawFirebar:
    a = M(TallEnemy);
    a >>= 1;
    a >>= 1;
    pha();
    a &= 0x01;
    a ^= 0x64;
    writeData(Sprite_Tilenumber + y, a);
    pla();
    a >>= 1;
    a >>= 1;
    a = 0x02;
    if (!c)
        goto FireA;
    a |= 0xc0;

FireA:
    writeData(Sprite_Attributes + y, a);
    goto Return;
    // ;-------------------------------------------------------------------------------------

DrawExplosion_Fireball:
    y = M(Alt_SprDataOffset + x);
    a = M(Fireball_State + x);
    ++M(Fireball_State + x);
    a >>= 1;
    a &= 0x07;
    compare(a, 0x03);
    if (c)
        goto KillFireBall;

DrawExplosion_Fireworks:
    x = a;
    a = M(ExplosionTiles + x);
    ++y;
    JSR(DumpFourSpr, 528);
    --y;
    x = M(BulletBill_FrenzyVar);
    a = M(Fireball_Rel_YPos);
    c = 1;
    a -= 0x04;
    writeData(Sprite_Y_Position + y, a);
    writeData(0x0208 + y, a);
    c = 0;
    a += 0x08;
    writeData(0x0204 + y, a);
    writeData(0x020c + y, a);
    a = M(Fireball_Rel_XPos);
    c = 1;
    a -= 0x04;
    writeData(Sprite_X_Position + y, a);
    writeData(0x0207 + y, a);
    c = 0;
    a += 0x08;
    writeData(0x020b + y, a);
    writeData(0x020f + y, a);
    a = 0x02;
    writeData(Sprite_Attributes + y, a);
    a = 0x82;
    writeData(0x0206 + y, a);
    a = 0x42;
    writeData(0x020a + y, a);
    a = 0xc2;
    writeData(0x020e + y, a);
    goto Return;

KillFireBall:
    a = 0x00;
    writeData(Fireball_State + x, a);
    goto Return;
    // ;-------------------------------------------------------------------------------------

DrawSmallPlatform:
    y = M(Enemy_SprDataOffset + x);
    a = 0x5b;
    ++y;
    JSR(DumpSixSpr, 529);
    ++y;
    a = 0x02;
    JSR(DumpSixSpr, 530);
    --y;
    --y;
    a = M(Enemy_Rel_XPos);
    writeData(Sprite_X_Position + y, a);
    writeData(0x020f + y, a);
    c = 0;
    a += 0x08;
    writeData(0x0207 + y, a);
    writeData(0x0213 + y, a);
    c = 0;
    a += 0x08;
    writeData(0x020b + y, a);
    writeData(0x0217 + y, a);
    a = M(Enemy_Y_Position + x);
    x = a;
    pha();
    compare(x, 0x20);
    if (c)
        goto TopSP;
    a = 0xf8;

TopSP:
    JSR(DumpThreeSpr, 531);
    pla();
    c = 0;
    a += 0x80;
    x = a;
    compare(x, 0x20);
    if (c)
        goto BotSP;
    a = 0xf8;

BotSP:
    writeData(0x020c + y, a);
    writeData(0x0210 + y, a);
    writeData(0x0214 + y, a);
    a = M(Enemy_OffscreenBits);
    pha();
    a &= 0x08;
    if (z)
        goto SOfs;
    a = 0xf8;
    writeData(Sprite_Y_Position + y, a);
    writeData(0x020c + y, a);

SOfs:
    pla();
    pha();
    a &= 0x04;
    if (z)
        goto SOfs2;
    a = 0xf8;
    writeData(0x0204 + y, a);
    writeData(0x0210 + y, a);

SOfs2:
    pla();
    a &= 0x02;
    if (z)
        goto ExSPl;
    a = 0xf8;
    writeData(0x0208 + y, a);
    writeData(0x0214 + y, a);

ExSPl:
    x = M(BulletBill_FrenzyVar);
    goto Return;
    // ;-------------------------------------------------------------------------------------

DrawBubble:
    y = M(Player_Y_HighPos);
    --y;
    if (!z)
        goto ExDBub;
    a = M(Bubble_OffscreenBits);
    a &= 0x08;
    if (!z)
        goto ExDBub;
    y = M(Bubble_SprDataOffset + x);
    a = M(Bubble_Rel_XPos);
    writeData(Sprite_X_Position + y, a);
    a = M(Bubble_Rel_YPos);
    writeData(Sprite_Y_Position + y, a);
    a = 0x74;
    writeData(Sprite_Tilenumber + y, a);
    a = 0x02;
    writeData(Sprite_Attributes + y, a);

ExDBub:
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$00 - used to store player's vertical offscreen bits
    // ;tiles arranged in order, 2 tiles per row, top to bottom
    // ;big player table
    // ;small player table
    // ;used by both player sizes

PlayerGfxHandler:
    a = M(InjuryTimer);
    if (z)
        goto CntPl;
    a = M(TallEnemy);
    a >>= 1;
    if (c)
        goto ExPGH;

CntPl:
    a = M(GreenParatroopaJump);
    compare(a, 0x0b);
    if (z)
        goto PlayerKilled;
    a = M(PlayerChangeSizeFlag);
    if (!z)
        goto DoChangeSize;
    y = M(SwimmingFlag);
    if (z)
        goto FindPlayerAction;
    a = M(Player_State);
    compare(a, 0x00);
    if (z)
        goto FindPlayerAction;
    JSR(FindPlayerAction, 532);
    a = M(TallEnemy);
    a &= 0x04;
    if (!z)
        goto ExPGH;
    x = a;
    y = M(Player_SprDataOffset);
    a = M(BulletBill_CannonVar);
    a >>= 1;
    if (c)
        goto SwimKT;
    ++y;
    ++y;
    ++y;
    ++y;

SwimKT:
    a = M(PlayerSize);
    if (z)
        goto BigKTS;
    a = M(0x0219 + y);
    compare(a, M(0xeeb5));
    if (z)
        goto ExPGH;
    ++x;

BigKTS:
    a = M(SwimKickTileNum + x);
    writeData(0x0219 + y, a);

ExPGH:
    goto Return;

FindPlayerAction:
    JSR(ProcessPlayerAction, 533);
    goto PlayerGfxProcessing;

DoChangeSize:
    JSR(HandleChangeSize, 534);
    goto PlayerGfxProcessing;

PlayerKilled:
    y = 0x0e;
    a = M(PlayerGfxTblOffsets + y);

PlayerGfxProcessing:
    writeData(PlayerGfxOffset, a);
    a = 0x04;
    JSR(RenderPlayerSub, 535);
    JSR(ChkForPlayerAttrib, 536);
    a = M(FireballThrowingTimer);
    if (z)
        goto PlayerOffscreenChk;
    y = 0x00;
    a = M(PlayerAnimTimer);
    compare(a, M(FireballThrowingTimer));
    writeData(FireballThrowingTimer, y);
    if (c)
        goto PlayerOffscreenChk;
    writeData(FireballThrowingTimer, a);
    y = 0x07;
    a = M(PlayerGfxTblOffsets + y);
    writeData(PlayerGfxOffset, a);
    y = 0x04;
    a = M(Player_X_Speed);
    a |= M(Podoboo);
    if (z)
        goto SUpdR;
    --y;

SUpdR:
    a = y;
    JSR(RenderPlayerSub, 537);

PlayerOffscreenChk:
    a = M(Player_OffscreenBits);
    a >>= 1;
    a >>= 1;
    a >>= 1;
    a >>= 1;
    writeData(TitleScreenModeValue, a);
    x = 0x03;
    a = M(Player_SprDataOffset);
    c = 0;
    a += 0x18;
    y = a;

PROfsLoop:
    a = 0xf8;
    M(TitleScreenModeValue) >>= 1;
    if (!c)
        goto NPROffscr;
    JSR(DumpTwoSpr, 538);

NPROffscr:
    a = y;
    c = 1;
    a -= 0x08;
    y = a;
    --x;
    if (!n)
        goto PROfsLoop;
    goto Return;
    // ;-------------------------------------------------------------------------------------

DrawPlayer_Intermediate:
    x = 0x05;

PIntLoop:
    a = M(IntermediatePlayerData + x);
    writeData(VictoryModeValue + x, a);
    --x;
    if (!n)
        goto PIntLoop;
    x = 0xb8;
    y = 0x04;
    JSR(DrawPlayerLoop, 539);
    a = M(0x0226);
    a |= 0x40;
    writeData(0x0222, a);
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$00-$01 - used to hold tile numbers, $00 also used to hold upper extent of animation frames
    // ;$02 - vertical position
    // ;$03 - facing direction, used as horizontal flip control
    // ;$04 - attributes
    // ;$05 - horizontal position
    // ;$07 - number of rows to draw
    // ;these also used in IntermediatePlayerData

RenderPlayerSub:
    writeData(World8, a);
    a = M(Player_Rel_XPos);
    writeData(Player_Pos_ForScroll, a);
    writeData(World6, a);
    a = M(Player_Rel_YPos);
    writeData(VictoryModeValue, a);
    a = M(BulletBill_CannonVar);
    writeData(GameOverModeValue, a);
    a = M(Player_SprAttrib);
    writeData(World5, a);
    x = M(PlayerGfxOffset);
    y = M(Player_SprDataOffset);

DrawPlayerLoop:
    a = M(PlayerGraphicsTable + x);
    writeData(TitleScreenModeValue, a);
    a = M(0xee18 + x);
    JSR(DrawOneSpriteRow, 540);
    --M(World8);
    if (!z)
        goto DrawPlayerLoop;
    goto Return;

ProcessPlayerAction:
    a = M(Player_State);
    compare(a, 0x03);
    if (z)
        goto ActionClimbing;
    compare(a, 0x02);
    if (z)
        goto ActionFalling;
    compare(a, 0x01);
    if (!z)
        goto ProcOnGroundActs;
    a = M(SwimmingFlag);
    if (!z)
        goto ActionSwimming;
    y = 0x06;
    a = M(CrouchingFlag);
    if (!z)
        goto NonAnimatedActs;
    y = 0x00;
    goto NonAnimatedActs;

ProcOnGroundActs:
    y = 0x06;
    a = M(CrouchingFlag);
    if (!z)
        goto NonAnimatedActs;
    y = 0x02;
    a = M(Player_X_Speed);
    a |= M(Podoboo);
    if (z)
        goto NonAnimatedActs;
    a = M(Player_XSpeedAbsolute);
    compare(a, 0x09);
    if (!c)
        goto ActionWalkRun;
    a = M(Player_MovingDir);
    a &= M(BulletBill_CannonVar);
    if (!z)
        goto ActionWalkRun;
    ++y;

NonAnimatedActs:
    JSR(GetGfxOffsetAdder, 541);
    a = 0x00;
    writeData(PlayerAnimCtrl, a);
    a = M(PlayerGfxTblOffsets + y);
    goto Return;

ActionFalling:
    y = 0x04;
    JSR(GetGfxOffsetAdder, 542);
    goto GetCurrentAnimOffset;

ActionWalkRun:
    y = 0x04;
    JSR(GetGfxOffsetAdder, 543);
    goto FourFrameExtent;

ActionClimbing:
    y = 0x05;
    a = M(Player_Y_Speed);
    if (z)
        goto NonAnimatedActs;
    JSR(GetGfxOffsetAdder, 544);
    goto ThreeFrameExtent;

ActionSwimming:
    y = 0x01;
    JSR(GetGfxOffsetAdder, 545);
    a = M(JumpSwimTimer);
    a |= M(PlayerAnimCtrl);
    if (!z)
        goto FourFrameExtent;
    a = M(GreyCheepCheep);
    a <<= 1;
    if (c)
        goto FourFrameExtent;

GetCurrentAnimOffset:
    a = M(PlayerAnimCtrl);
    goto GetOffsetFromAnimCtrl;

FourFrameExtent:
    a = 0x03;
    goto AnimationControl;

ThreeFrameExtent:
    a = 0x02;

AnimationControl:
    writeData(TitleScreenModeValue, a);
    JSR(GetCurrentAnimOffset, 546);
    pha();
    a = M(PlayerAnimTimer);
    if (!z)
        goto ExAnimC;
    a = M(PlayerAnimTimerSet);
    writeData(PlayerAnimTimer, a);
    a = M(PlayerAnimCtrl);
    c = 0;
    a += 0x01;
    compare(a, M(TitleScreenModeValue));
    if (!c)
        goto SetAnimC;
    a = 0x00;

SetAnimC:
    writeData(PlayerAnimCtrl, a);

ExAnimC:
    pla();
    goto Return;

GetGfxOffsetAdder:
    a = M(PlayerSize);
    if (z)
        goto SzOfs;
    a = y;
    c = 0;
    a += 0x08;
    y = a;

SzOfs:
    goto Return;

HandleChangeSize:
    y = M(PlayerAnimCtrl);
    a = M(TallEnemy);
    a &= 0x03;
    if (!z)
        goto GorSLog;
    ++y;
    compare(y, 0x0a);
    if (!c)
        goto CSzNext;
    y = 0x00;
    writeData(PlayerChangeSizeFlag, y);

CSzNext:
    writeData(PlayerAnimCtrl, y);

GorSLog:
    a = M(PlayerSize);
    if (!z)
        goto ShrinkPlayer;
    a = M(ChangeSizeOffsetAdder + y);
    y = 0x0f;

GetOffsetFromAnimCtrl:
    a <<= 1;
    a <<= 1;
    a <<= 1;
    a += M(PlayerGfxTblOffsets + y);
    goto Return;

ShrinkPlayer:
    a = y;
    c = 0;
    a += 0x0a;
    x = a;
    y = 0x09;
    a = M(ChangeSizeOffsetAdder + x);
    if (!z)
        goto ShrPlF;
    y = 0x01;

ShrPlF:
    a = M(PlayerGfxTblOffsets + y);
    goto Return;

ChkForPlayerAttrib:
    y = M(Player_SprDataOffset);
    a = M(GreenParatroopaJump);
    compare(a, 0x0b);
    if (z)
        goto KilledAtt;
    a = M(PlayerGfxOffset);
    compare(a, 0x50);
    if (z)
        goto C_S_IGAtt;
    compare(a, 0xb8);
    if (z)
        goto C_S_IGAtt;
    compare(a, 0xc0);
    if (z)
        goto C_S_IGAtt;
    compare(a, 0xc8);
    if (!z)
        goto ExPlyrAt;

KilledAtt:
    a = M(0x0212 + y);
    a &= 0x3f;
    writeData(0x0212 + y, a);
    a = M(0x0216 + y);
    a &= 0x3f;
    a |= 0x40;
    writeData(0x0216 + y, a);

C_S_IGAtt:
    a = M(0x021a + y);
    a &= 0x3f;
    writeData(0x021a + y, a);
    a = M(0x021e + y);
    a &= 0x3f;
    a |= 0x40;
    writeData(0x021e + y, a);

ExPlyrAt:
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$00 - used in adding to get proper offset

RelativePlayerPosition:
    x = 0x00;
    y = 0x00;
    goto RelWOfs;

RelativeBubblePosition:
    y = 0x01;
    JSR(GetProperObjOffset, 547);
    y = 0x03;
    goto RelWOfs;

RelativeFireballPosition:
    y = 0x00;
    JSR(GetProperObjOffset, 548);
    y = 0x02;

RelWOfs:
    JSR(GetObjRelativePosition, 549);
    x = M(BulletBill_FrenzyVar);
    goto Return;

RelativeMiscPosition:
    y = 0x02;
    JSR(GetProperObjOffset, 550);
    y = 0x06;
    goto RelWOfs;

RelativeEnemyPosition:
    a = 0x01;
    y = 0x01;
    goto VariableObjOfsRelPos;

RelativeBlockPosition:
    a = 0x09;
    y = 0x04;
    JSR(VariableObjOfsRelPos, 551);
    ++x;
    ++x;
    a = 0x09;
    ++y;

VariableObjOfsRelPos:
    writeData(TitleScreenModeValue, x);
    c = 0;
    a += M(TitleScreenModeValue);
    x = a;
    JSR(GetObjRelativePosition, 552);
    x = M(BulletBill_FrenzyVar);
    goto Return;

GetObjRelativePosition:
    a = M(Player_Y_Position + x);
    writeData(Player_Rel_YPos + y, a);
    a = M(Player_X_Position + x);
    c = 1;
    a -= M(ScreenLeft_X_Pos);
    writeData(Player_Rel_XPos + y, a);
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$00 - used as temp variable to hold offscreen bits

GetPlayerOffscreenBits:
    x = 0x00;
    y = 0x00;
    goto GetOffScreenBitsSet;

GetFireballOffscreenBits:
    y = 0x00;
    JSR(GetProperObjOffset, 553);
    y = 0x02;
    goto GetOffScreenBitsSet;

GetBubbleOffscreenBits:
    y = 0x01;
    JSR(GetProperObjOffset, 554);
    y = 0x03;
    goto GetOffScreenBitsSet;

GetMiscOffscreenBits:
    y = 0x02;
    JSR(GetProperObjOffset, 555);
    y = 0x06;
    goto GetOffScreenBitsSet;

GetProperObjOffset:
    a = x;
    c = 0;
    a += M(ObjOffsetData + y);
    x = a;
    goto Return;

GetEnemyOffscreenBits:
    a = 0x01;
    y = 0x01;
    goto SetOffscrBitsOffset;

GetBlockOffscreenBits:
    a = 0x09;
    y = 0x04;

SetOffscrBitsOffset:
    writeData(TitleScreenModeValue, x);
    c = 0;
    a += M(TitleScreenModeValue);
    x = a;

GetOffScreenBitsSet:
    a = y;
    pha();
    JSR(RunOffscrBitsSubs, 556);
    a <<= 1;
    a <<= 1;
    a <<= 1;
    a <<= 1;
    a |= M(TitleScreenModeValue);
    writeData(TitleScreenModeValue, a);
    pla();
    y = a;
    a = M(TitleScreenModeValue);
    writeData(Player_OffscreenBits + y, a);
    x = M(BulletBill_FrenzyVar);
    goto Return;

RunOffscrBitsSubs:
    JSR(GetXOffscreenBits, 557);
    a >>= 1;
    a >>= 1;
    a >>= 1;
    a >>= 1;
    writeData(TitleScreenModeValue, a);
    goto GetYOffscreenBits;
    // ;--------------------------------
    // ;(these apply to these three subsections)
    // ;$04 - used to store proper offset
    // ;$05 - used as adder in DividePDiff
    // ;$06 - used to store preset value used to compare to pixel difference in $07
    // ;$07 - used to store difference between coordinates of object and screen edges

GetXOffscreenBits:
    writeData(World5, x);
    y = 0x01;

XOfsLoop:
    a = M(ScreenLeft_X_Pos + y);
    c = 1;
    a -= M(Player_X_Position + x);
    writeData(World8, a);
    a = M(ScreenLeft_PageLoc + y);
    a -= M(Player_PageLoc + x);
    x = M(DefaultXOnscreenOfs + y);
    compare(a, 0x00);
    if (n)
        goto XLdBData;
    x = M(0xf1f4 + y);
    compare(a, 0x01);
    if (!n)
        goto XLdBData;
    a = 0x38;
    writeData(World7, a);
    a = 0x08;
    JSR(DividePDiff, 558);

XLdBData:
    a = M(XOffscreenBitsData + x);
    x = M(World5);
    compare(a, 0x00);
    if (!z)
        goto ExXOfsBS;
    --y;
    if (!n)
        goto XOfsLoop;

ExXOfsBS:
    goto Return;
    // ;--------------------------------

GetYOffscreenBits:
    writeData(World5, x);
    y = 0x01;

YOfsLoop:
    a = M(HighPosUnitData + y);
    c = 1;
    a -= M(Player_Y_Position + x);
    writeData(World8, a);
    a = 0x01;
    a -= M(Player_Y_HighPos + x);
    x = M(DefaultYOnscreenOfs + y);
    compare(a, 0x00);
    if (n)
        goto YLdBData;
    x = M(0xf235 + y);
    compare(a, 0x01);
    if (!n)
        goto YLdBData;
    a = 0x20;
    writeData(World7, a);
    a = 0x04;
    JSR(DividePDiff, 559);

YLdBData:
    a = M(YOffscreenBitsData + x);
    x = M(World5);
    compare(a, 0x00);
    if (!z)
        goto ExYOfsBS;
    --y;
    if (!n)
        goto YOfsLoop;

ExYOfsBS:
    goto Return;
    // ;--------------------------------

DividePDiff:
    writeData(World6, a);
    a = M(World8);
    compare(a, M(World7));
    if (c)
        goto ExDivPD;
    a >>= 1;
    a >>= 1;
    a >>= 1;
    a &= 0x07;
    compare(y, 0x01);
    if (c)
        goto SetOscrO;
    a += M(World6);

SetOscrO:
    x = a;

ExDivPD:
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;$00-$01 - tile numbers
    // ;$02 - Y coordinate
    // ;$03 - flip control
    // ;$04 - sprite attributes
    // ;$05 - X coordinate

DrawSpriteObject:
    a = M(GameOverModeValue);
    a >>= 1;
    a >>= 1;
    a = M(TitleScreenModeValue);
    if (!c)
        goto NoHFlip;
    writeData(0x0205 + y, a);
    a = M(GameModeValue);
    writeData(Sprite_Tilenumber + y, a);
    a = 0x40;
    if (!z)
        goto SetHFAt;

NoHFlip:
    writeData(Sprite_Tilenumber + y, a);
    a = M(GameModeValue);
    writeData(0x0205 + y, a);
    a = 0x00;

SetHFAt:
    a |= M(World5);
    writeData(Sprite_Attributes + y, a);
    writeData(0x0206 + y, a);
    a = M(VictoryModeValue);
    writeData(Sprite_Y_Position + y, a);
    writeData(0x0204 + y, a);
    a = M(World6);
    writeData(Sprite_X_Position + y, a);
    c = 0;
    a += 0x08;
    writeData(0x0207 + y, a);
    a = M(VictoryModeValue);
    c = 0;
    a += 0x08;
    writeData(VictoryModeValue, a);
    a = y;
    c = 0;
    a += 0x08;
    y = a;
    ++x;
    ++x;
    goto Return;
    // ;-------------------------------------------------------------------------------------
    // ;unused space
    // ;-------------------------------------------------------------------------------------

SoundEngine:
    a = M(OperMode);
    if (!z)
        goto SndOn;
    writeData(SND_MASTERCTRL_REG, a);
    goto Return;

SndOn:
    a = 0xff;
    writeData(JOYPAD_PORT2, a);
    a = 0x0f;
    writeData(SND_MASTERCTRL_REG, a);
    a = M(PauseModeFlag);
    if (!z)
        goto InPause;
    a = M(PauseSoundQueue);
    compare(a, 0x01);
    if (!z)
        goto RunSoundSubroutines;

InPause:
    a = M(PauseSoundBuffer);
    if (!z)
        goto ContPau;
    a = M(PauseSoundQueue);
    if (z)
        goto SkipSoundSubroutines;
    writeData(PauseSoundBuffer, a);
    writeData(PauseModeFlag, a);
    a = 0x00;
    writeData(SND_MASTERCTRL_REG, a);
    writeData(Square1SoundBuffer, a);
    writeData(Square2SoundBuffer, a);
    writeData(NoiseSoundBuffer, a);
    a = 0x0f;
    writeData(SND_MASTERCTRL_REG, a);
    a = 0x2a;
    writeData(Squ1_SfxLenCounter, a);

PTone1F:
    a = 0x44;
    if (!z)
        goto PTRegC;

ContPau:
    a = M(Squ1_SfxLenCounter);
    compare(a, 0x24);
    if (z)
        goto PTone2F;
    compare(a, 0x1e);
    if (z)
        goto PTone1F;
    compare(a, 0x18);
    if (!z)
        goto DecPauC;

PTone2F:
    a = 0x64;

PTRegC:
    x = 0x84;
    y = 0x7f;
    JSR(PlaySqu1Sfx, 560);

DecPauC:
    --M(Squ1_SfxLenCounter);
    if (!z)
        goto SkipSoundSubroutines;
    a = 0x00;
    writeData(SND_MASTERCTRL_REG, a);
    a = M(PauseSoundBuffer);
    compare(a, 0x02);
    if (!z)
        goto SkipPIn;
    a = 0x00;
    writeData(PauseModeFlag, a);

SkipPIn:
    a = 0x00;
    writeData(PauseSoundBuffer, a);
    if (z)
        goto SkipSoundSubroutines;

RunSoundSubroutines:
    JSR(Square1SfxHandler, 561);
    JSR(Square2SfxHandler, 562);
    JSR(NoiseSfxHandler, 563);
    JSR(MusicHandler, 564);
    a = 0x00;
    writeData(AreaMusicQueue, a);
    writeData(EventMusicQueue, a);

SkipSoundSubroutines:
    a = 0x00;
    writeData(Square1SoundQueue, a);
    writeData(Square2SoundQueue, a);
    writeData(NoiseSoundQueue, a);
    writeData(PauseSoundQueue, a);
    y = M(DAC_Counter);
    a = M(AreaMusicBuffer);
    a &= 0x03;
    if (z)
        goto NoIncDAC;
    ++M(DAC_Counter);
    compare(y, 0x30);
    if (!c)
        goto StrWave;

NoIncDAC:
    a = y;
    if (z)
        goto StrWave;
    --M(DAC_Counter);

StrWave:
    writeData(0x4011, y);
    goto Return;
    // ;--------------------------------

Dump_Squ1_Regs:
    writeData(0x4001, y);
    writeData(SND_SQUARE1_REG, x);
    goto Return;

PlaySqu1Sfx:
    JSR(Dump_Squ1_Regs, 565);

SetFreq_Squ1:
    x = 0x00;

Dump_Freq_Regs:
    y = a;
    a = M(0xff01 + y);
    if (z)
        goto NoTone;
    writeData(0x4002 + x, a);
    a = M(FreqRegLookupTbl + y);
    a |= 0x08;
    writeData(0x4003 + x, a);

NoTone:
    goto Return;

Dump_Sq2_Regs:
    writeData(SND_SQUARE2_REG, x);
    writeData(0x4005, y);
    goto Return;

PlaySqu2Sfx:
    JSR(Dump_Sq2_Regs, 566);

SetFreq_Squ2:
    x = 0x04;
    if (!z)
        goto Dump_Freq_Regs;

SetFreq_Tri:
    x = 0x08;
    if (!z)
        goto Dump_Freq_Regs;
    // ;--------------------------------

PlayFlagpoleSlide:
    a = 0x40;
    writeData(Squ1_SfxLenCounter, a);
    a = 0x62;
    JSR(SetFreq_Squ1, 567);
    x = 0x99;
    if (!z)
        goto FPS2nd;

PlaySmallJump:
    a = 0x26;
    if (!z)
        goto JumpRegContents;

PlayBigJump:
    a = 0x18;

JumpRegContents:
    x = 0x82;
    y = 0xa7;
    JSR(PlaySqu1Sfx, 568);
    a = 0x28;
    writeData(Squ1_SfxLenCounter, a);

ContinueSndJump:
    a = M(Squ1_SfxLenCounter);
    compare(a, 0x25);
    if (!z)
        goto N2Prt;
    x = 0x5f;
    y = 0xf6;
    if (!z)
        goto DmpJpFPS;

N2Prt:
    compare(a, 0x20);
    if (!z)
        goto DecJpFPS;
    x = 0x48;

FPS2nd:
    y = 0xbc;

DmpJpFPS:
    JSR(Dump_Squ1_Regs, 569);
    if (!z)
        goto DecJpFPS;

PlayFireballThrow:
    a = 0x05;
    y = 0x99;
    if (!z)
        goto Fthrow;

PlayBump:
    a = 0x0a;
    y = 0x93;

Fthrow:
    x = 0x9e;
    writeData(Squ1_SfxLenCounter, a);
    a = 0x0c;
    JSR(PlaySqu1Sfx, 570);

ContinueBumpThrow:
    a = M(Squ1_SfxLenCounter);
    compare(a, 0x06);
    if (!z)
        goto DecJpFPS;
    a = 0xbb;
    writeData(0x4001, a);

DecJpFPS:
    if (!z)
        goto BranchToDecLength1;

Square1SfxHandler:
    y = M(Square1SoundQueue);
    if (z)
        goto CheckSfx1Buffer;
    writeData(Square1SoundBuffer, y);
    if (n)
        goto PlaySmallJump;
    M(Square1SoundQueue) >>= 1;
    if (c)
        goto PlayBigJump;
    M(Square1SoundQueue) >>= 1;
    if (c)
        goto PlayBump;
    M(Square1SoundQueue) >>= 1;
    if (c)
        goto PlaySwimStomp;
    M(Square1SoundQueue) >>= 1;
    if (c)
        goto PlaySmackEnemy;
    M(Square1SoundQueue) >>= 1;
    if (c)
        goto PlayPipeDownInj;
    M(Square1SoundQueue) >>= 1;
    if (c)
        goto PlayFireballThrow;
    M(Square1SoundQueue) >>= 1;
    if (c)
        goto PlayFlagpoleSlide;

CheckSfx1Buffer:
    a = M(Square1SoundBuffer);
    if (z)
        goto ExS1H;
    if (n)
        goto ContinueSndJump;
    a >>= 1;
    if (c)
        goto ContinueSndJump;
    a >>= 1;
    if (c)
        goto ContinueBumpThrow;
    a >>= 1;
    if (c)
        goto ContinueSwimStomp;
    a >>= 1;
    if (c)
        goto ContinueSmackEnemy;
    a >>= 1;
    if (c)
        goto ContinuePipeDownInj;
    a >>= 1;
    if (c)
        goto ContinueBumpThrow;
    a >>= 1;
    if (c)
        goto DecrementSfx1Length;

ExS1H:
    goto Return;

PlaySwimStomp:
    a = 0x0e;
    writeData(Squ1_SfxLenCounter, a);
    y = 0x9c;
    x = 0x9e;
    a = 0x26;
    JSR(PlaySqu1Sfx, 571);

ContinueSwimStomp:
    y = M(Squ1_SfxLenCounter);
    a = M(0xf3b0 + y);
    writeData(SND_SQUARE1_REG, a);
    compare(y, 0x06);
    if (!z)
        goto BranchToDecLength1;
    a = 0x9e;
    writeData(0x4002, a);

BranchToDecLength1:
    if (!z)
        goto DecrementSfx1Length;

PlaySmackEnemy:
    a = 0x0e;
    y = 0xcb;
    x = 0x9f;
    writeData(Squ1_SfxLenCounter, a);
    a = 0x28;
    JSR(PlaySqu1Sfx, 572);
    if (!z)
        goto DecrementSfx1Length;

ContinueSmackEnemy:
    y = M(Squ1_SfxLenCounter);
    compare(y, 0x08);
    if (!z)
        goto SmSpc;
    a = 0xa0;
    writeData(0x4002, a);
    a = 0x9f;
    if (!z)
        goto SmTick;

SmSpc:
    a = 0x90;

SmTick:
    writeData(SND_SQUARE1_REG, a);

DecrementSfx1Length:
    --M(Squ1_SfxLenCounter);
    if (!z)
        goto ExSfx1;

StopSquare1Sfx:
    x = 0x00;
    writeData(Square1SoundBuffer, x);
    x = 0x0e;
    writeData(SND_MASTERCTRL_REG, x);
    x = 0x0f;
    writeData(SND_MASTERCTRL_REG, x);

ExSfx1:
    goto Return;

PlayPipeDownInj:
    a = 0x2f;
    writeData(Squ1_SfxLenCounter, a);

ContinuePipeDownInj:
    a = M(Squ1_SfxLenCounter);
    a >>= 1;
    if (c)
        goto NoPDwnL;
    a >>= 1;
    if (c)
        goto NoPDwnL;
    a &= 0x02;
    if (z)
        goto NoPDwnL;
    y = 0x91;
    x = 0x9a;
    a = 0x44;
    JSR(PlaySqu1Sfx, 573);

NoPDwnL:
    goto DecrementSfx1Length;
    // ;--------------------------------
    // ;residual frequency data

PlayCoinGrab:
    a = 0x35;
    x = 0x8d;
    if (!z)
        goto CGrab_TTickRegL;

PlayTimerTick:
    a = 0x06;
    x = 0x98;

CGrab_TTickRegL:
    writeData(Squ2_SfxLenCounter, a);
    y = 0x7f;
    a = 0x42;
    JSR(PlaySqu2Sfx, 574);

ContinueCGrabTTick:
    a = M(Squ2_SfxLenCounter);
    compare(a, 0x30);
    if (!z)
        goto N2Tone;
    a = 0x54;
    writeData(0x4006, a);

N2Tone:
    if (!z)
        goto DecrementSfx2Length;

PlayBlast:
    a = 0x20;
    writeData(Squ2_SfxLenCounter, a);
    y = 0x94;
    a = 0x5e;
    if (!z)
        goto SBlasJ;

ContinueBlast:
    a = M(Squ2_SfxLenCounter);
    compare(a, 0x18);
    if (!z)
        goto DecrementSfx2Length;
    y = 0x93;
    a = 0x18;

SBlasJ:
    if (!z)
        goto BlstSJp;

PlayPowerUpGrab:
    a = 0x36;
    writeData(Squ2_SfxLenCounter, a);

ContinuePowerUpGrab:
    a = M(Squ2_SfxLenCounter);
    a >>= 1;
    if (c)
        goto DecrementSfx2Length;
    y = a;
    a = M(0xf4d9 + y);
    x = 0x5d;
    y = 0x7f;

LoadSqu2Regs:
    JSR(PlaySqu2Sfx, 575);

DecrementSfx2Length:
    --M(Squ2_SfxLenCounter);
    if (!z)
        goto ExSfx2;

EmptySfx2Buffer:
    x = 0x00;
    writeData(Square2SoundBuffer, x);

StopSquare2Sfx:
    x = 0x0d;
    writeData(SND_MASTERCTRL_REG, x);
    x = 0x0f;
    writeData(SND_MASTERCTRL_REG, x);

ExSfx2:
    goto Return;

Square2SfxHandler:
    a = M(Square2SoundBuffer);
    a &= 0x40;
    if (!z)
        goto ContinueExtraLife;
    y = M(Square2SoundQueue);
    if (z)
        goto CheckSfx2Buffer;
    writeData(Square2SoundBuffer, y);
    if (n)
        goto PlayBowserFall;
    M(Square2SoundQueue) >>= 1;
    if (c)
        goto PlayCoinGrab;
    M(Square2SoundQueue) >>= 1;
    if (c)
        goto PlayGrowPowerUp;
    M(Square2SoundQueue) >>= 1;
    if (c)
        goto PlayGrowVine;
    M(Square2SoundQueue) >>= 1;
    if (c)
        goto PlayBlast;
    M(Square2SoundQueue) >>= 1;
    if (c)
        goto PlayTimerTick;
    M(Square2SoundQueue) >>= 1;
    if (c)
        goto PlayPowerUpGrab;
    M(Square2SoundQueue) >>= 1;
    if (c)
        goto PlayExtraLife;

CheckSfx2Buffer:
    a = M(Square2SoundBuffer);
    if (z)
        goto ExS2H;
    if (n)
        goto ContinueBowserFall;
    a >>= 1;
    if (c)
        goto Cont_CGrab_TTick;
    a >>= 1;
    if (c)
        goto ContinueGrowItems;
    a >>= 1;
    if (c)
        goto ContinueGrowItems;
    a >>= 1;
    if (c)
        goto ContinueBlast;
    a >>= 1;
    if (c)
        goto Cont_CGrab_TTick;
    a >>= 1;
    if (c)
        goto ContinuePowerUpGrab;
    a >>= 1;
    if (c)
        goto ContinueExtraLife;

ExS2H:
    goto Return;

Cont_CGrab_TTick:
    goto ContinueCGrabTTick;

JumpToDecLength2:
    goto DecrementSfx2Length;

PlayBowserFall:
    a = 0x38;
    writeData(Squ2_SfxLenCounter, a);
    y = 0xc4;
    a = 0x18;

BlstSJp:
    if (!z)
        goto PBFRegs;

ContinueBowserFall:
    a = M(Squ2_SfxLenCounter);
    compare(a, 0x08);
    if (!z)
        goto DecrementSfx2Length;
    y = 0xa4;
    a = 0x5a;

PBFRegs:
    x = 0x9f;

EL_LRegs:
    if (!z)
        goto LoadSqu2Regs;

PlayExtraLife:
    a = 0x30;
    writeData(Squ2_SfxLenCounter, a);

ContinueExtraLife:
    a = M(Squ2_SfxLenCounter);
    x = 0x03;

DivLLoop:
    a >>= 1;
    if (c)
        goto JumpToDecLength2;
    --x;
    if (!z)
        goto DivLLoop;
    y = a;
    a = M(0xf4d3 + y);
    x = 0x82;
    y = 0x7f;
    if (!z)
        goto EL_LRegs;

PlayGrowPowerUp:
    a = 0x10;
    if (!z)
        goto GrowItemRegs;

PlayGrowVine:
    a = 0x20;

GrowItemRegs:
    writeData(Squ2_SfxLenCounter, a);
    a = 0x7f;
    writeData(0x4005, a);
    a = 0x00;
    writeData(Sfx_SecondaryCounter, a);

ContinueGrowItems:
    ++M(Sfx_SecondaryCounter);
    a = M(Sfx_SecondaryCounter);
    a >>= 1;
    y = a;
    compare(y, M(Squ2_SfxLenCounter));
    if (z)
        goto StopGrowItems;
    a = 0x9d;
    writeData(SND_SQUARE2_REG, a);
    a = M(PUp_VGrow_FreqData + y);
    JSR(SetFreq_Squ2, 576);
    goto Return;

StopGrowItems:
    goto EmptySfx2Buffer;
    // ;--------------------------------

PlayBrickShatter:
    a = 0x20;
    writeData(Noise_SfxLenCounter, a);

ContinueBrickShatter:
    a = M(Noise_SfxLenCounter);
    a >>= 1;
    if (!c)
        goto DecrementSfx3Length;
    y = a;
    x = M(BrickShatterFreqData + y);
    a = M(BrickShatterEnvData + y);

PlayNoiseSfx:
    writeData(SND_NOISE_REG, a);
    writeData(0x400e, x);
    a = 0x18;
    writeData(0x400f, a);

DecrementSfx3Length:
    --M(Noise_SfxLenCounter);
    if (!z)
        goto ExSfx3;
    a = 0xf0;
    writeData(SND_NOISE_REG, a);
    a = 0x00;
    writeData(NoiseSoundBuffer, a);

ExSfx3:
    goto Return;

NoiseSfxHandler:
    y = M(NoiseSoundQueue);
    if (z)
        goto CheckNoiseBuffer;
    writeData(NoiseSoundBuffer, y);
    M(NoiseSoundQueue) >>= 1;
    if (c)
        goto PlayBrickShatter;
    M(NoiseSoundQueue) >>= 1;
    if (c)
        goto PlayBowserFlame;

CheckNoiseBuffer:
    a = M(NoiseSoundBuffer);
    if (z)
        goto ExNH;
    a >>= 1;
    if (c)
        goto ContinueBrickShatter;
    a >>= 1;
    if (c)
        goto ContinueBowserFlame;

ExNH:
    goto Return;

PlayBowserFlame:
    a = 0x40;
    writeData(Noise_SfxLenCounter, a);

ContinueBowserFlame:
    a = M(Noise_SfxLenCounter);
    a >>= 1;
    y = a;
    x = 0x0f;
    a = M(0xffc9 + y);
    if (!z)
        goto PlayNoiseSfx;
    // ;--------------------------------

ContinueMusic:
    goto HandleSquare2Music;

MusicHandler:
    a = M(EventMusicQueue);
    if (!z)
        goto LoadEventMusic;
    a = M(AreaMusicQueue);
    if (!z)
        goto LoadAreaMusic;
    a = M(EventMusicBuffer);
    a |= M(AreaMusicBuffer);
    if (!z)
        goto ContinueMusic;
    goto Return;

LoadEventMusic:
    writeData(EventMusicBuffer, a);
    compare(a, 0x01);
    if (!z)
        goto NoStopSfx;
    JSR(StopSquare1Sfx, 577);
    JSR(StopSquare2Sfx, 578);

NoStopSfx:
    x = M(AreaMusicBuffer);
    writeData(AreaMusicBuffer_Alt, x);
    y = 0x00;
    writeData(NoteLengthTblAdder, y);
    writeData(AreaMusicBuffer, y);
    compare(a, 0x40);
    if (!z)
        goto FindEventMusicHeader;
    x = 0x08;
    writeData(NoteLengthTblAdder, x);
    if (!z)
        goto FindEventMusicHeader;

LoadAreaMusic:
    compare(a, 0x04);
    if (!z)
        goto NoStop1;
    JSR(StopSquare1Sfx, 579);

NoStop1:
    y = 0x10;

GMLoopB:
    writeData(GroundMusicHeaderOfs, y);

HandleAreaMusicLoopB:
    y = 0x00;
    writeData(EventMusicBuffer, y);
    writeData(AreaMusicBuffer, a);
    compare(a, 0x01);
    if (!z)
        goto FindAreaMusicHeader;
    ++M(GroundMusicHeaderOfs);
    y = M(GroundMusicHeaderOfs);
    compare(y, 0x32);
    if (!z)
        goto LoadHeader;
    y = 0x11;
    if (!z)
        goto GMLoopB;

FindAreaMusicHeader:
    y = 0x08;
    writeData(MusicOffset_Square2, y);

FindEventMusicHeader:
    ++y;
    a >>= 1;
    if (!c)
        goto FindEventMusicHeader;

LoadHeader:
    a = M(0xf90c + y);
    y = a;
    a = M(MusicHeaderData + y);
    writeData(NoteLenLookupTblOfs, a);
    a = M(data_F90E + y);
    writeData(MusicDataLow, a);
    a = M(data_F90F + y);
    writeData(MusicDataHigh, a);
    a = M(data_F910 + y);
    writeData(MusicOffset_Triangle, a);
    a = M(data_F911 + y);
    writeData(MusicOffset_Square1, a);
    a = M(data_F912 + y);
    writeData(SoundMemory, a);
    writeData(NoiseDataLoopbackOfs, a);
    a = 0x01;
    writeData(Squ2_NoteLenCounter, a);
    writeData(Squ1_NoteLenCounter, a);
    writeData(Tri_NoteLenCounter, a);
    writeData(Noise_BeatLenCounter, a);
    a = 0x00;
    writeData(MusicOffset_Square2, a);
    writeData(AltRegContentFlag, a);
    a = 0x0b;
    writeData(SND_MASTERCTRL_REG, a);
    a = 0x0f;
    writeData(SND_MASTERCTRL_REG, a);

HandleSquare2Music:
    --M(Squ2_NoteLenCounter);
    if (!z)
        goto MiscSqu2MusicTasks;
    y = M(MusicOffset_Square2);
    ++M(MusicOffset_Square2);
    a = M(W(MusicDataLow) + y);
    if (z)
        goto EndOfMusicData;
    if (!n)
        goto Squ2NoteHandler;
    if (!z)
        goto Squ2LengthHandler;

EndOfMusicData:
    a = M(EventMusicBuffer);
    compare(a, 0x40);
    if (!z)
        goto NotTRO;
    a = M(AreaMusicBuffer_Alt);
    if (!z)
        goto MusicLoopBack;

NotTRO:
    a &= 0x04;
    if (!z)
        goto VictoryMLoopBack;
    a = M(AreaMusicBuffer);
    a &= 0x5f;
    if (!z)
        goto MusicLoopBack;
    a = 0x00;
    writeData(AreaMusicBuffer, a);
    writeData(EventMusicBuffer, a);
    writeData(SND_TRIANGLE_REG, a);
    a = 0x90;
    writeData(SND_SQUARE1_REG, a);
    writeData(SND_SQUARE2_REG, a);
    goto Return;

MusicLoopBack:
    goto HandleAreaMusicLoopB;

VictoryMLoopBack:
    goto LoadEventMusic;

Squ2LengthHandler:
    JSR(ProcessLengthData, 580);
    writeData(Squ2_NoteLenBuffer, a);
    y = M(MusicOffset_Square2);
    ++M(MusicOffset_Square2);
    a = M(W(MusicDataLow) + y);

Squ2NoteHandler:
    x = M(Square2SoundBuffer);
    if (!z)
        goto SkipFqL1;
    JSR(SetFreq_Squ2, 581);
    if (z)
        goto Rest;
    JSR(LoadControlRegs, 582);

Rest:
    writeData(Squ2_EnvelopeDataCtrl, a);
    JSR(Dump_Sq2_Regs, 583);

SkipFqL1:
    a = M(Squ2_NoteLenBuffer);
    writeData(Squ2_NoteLenCounter, a);

MiscSqu2MusicTasks:
    a = M(Square2SoundBuffer);
    if (!z)
        goto HandleSquare1Music;
    a = M(EventMusicBuffer);
    a &= 0x91;
    if (!z)
        goto HandleSquare1Music;
    y = M(Squ2_EnvelopeDataCtrl);
    if (z)
        goto NoDecEnv1;
    --M(Squ2_EnvelopeDataCtrl);

NoDecEnv1:
    JSR(LoadEnvelopeData, 584);
    writeData(SND_SQUARE2_REG, a);
    x = 0x7f;
    writeData(0x4005, x);

HandleSquare1Music:
    y = M(MusicOffset_Square1);
    if (z)
        goto HandleTriangleMusic;
    --M(Squ1_NoteLenCounter);
    if (!z)
        goto MiscSqu1MusicTasks;

FetchSqu1MusicData:
    y = M(MusicOffset_Square1);
    ++M(MusicOffset_Square1);
    a = M(W(MusicDataLow) + y);
    if (!z)
        goto Squ1NoteHandler;
    a = 0x83;
    writeData(SND_SQUARE1_REG, a);
    a = 0x94;
    writeData(0x4001, a);
    writeData(AltRegContentFlag, a);
    if (!z)
        goto FetchSqu1MusicData;

Squ1NoteHandler:
    JSR(AlternateLengthHandler, 585);
    writeData(Squ1_NoteLenCounter, a);
    y = M(Square1SoundBuffer);
    if (!z)
        goto HandleTriangleMusic;
    a = x;
    a &= 0x3e;
    JSR(SetFreq_Squ1, 586);
    if (z)
        goto SkipCtrlL;
    JSR(LoadControlRegs, 587);

SkipCtrlL:
    writeData(Squ1_EnvelopeDataCtrl, a);
    JSR(Dump_Squ1_Regs, 588);

MiscSqu1MusicTasks:
    a = M(Square1SoundBuffer);
    if (!z)
        goto HandleTriangleMusic;
    a = M(EventMusicBuffer);
    a &= 0x91;
    if (!z)
        goto DeathMAltReg;
    y = M(Squ1_EnvelopeDataCtrl);
    if (z)
        goto NoDecEnv2;
    --M(Squ1_EnvelopeDataCtrl);

NoDecEnv2:
    JSR(LoadEnvelopeData, 589);
    writeData(SND_SQUARE1_REG, a);

DeathMAltReg:
    a = M(AltRegContentFlag);
    if (!z)
        goto DoAltLoad;
    a = 0x7f;

DoAltLoad:
    writeData(0x4001, a);

HandleTriangleMusic:
    a = M(MusicOffset_Triangle);
    --M(Tri_NoteLenCounter);
    if (!z)
        goto HandleNoiseMusic;
    y = M(MusicOffset_Triangle);
    ++M(MusicOffset_Triangle);
    a = M(W(MusicDataLow) + y);
    if (z)
        goto LoadTriCtrlReg;
    if (!n)
        goto TriNoteHandler;
    JSR(ProcessLengthData, 590);
    writeData(Tri_NoteLenBuffer, a);
    a = 0x1f;
    writeData(SND_TRIANGLE_REG, a);
    y = M(MusicOffset_Triangle);
    ++M(MusicOffset_Triangle);
    a = M(W(MusicDataLow) + y);
    if (z)
        goto LoadTriCtrlReg;

TriNoteHandler:
    JSR(SetFreq_Tri, 591);
    x = M(Tri_NoteLenBuffer);
    writeData(Tri_NoteLenCounter, x);
    a = M(EventMusicBuffer);
    a &= 0x6e;
    if (!z)
        goto NotDOrD4;
    a = M(AreaMusicBuffer);
    a &= 0x0a;
    if (z)
        goto HandleNoiseMusic;

NotDOrD4:
    a = x;
    compare(a, 0x12);
    if (c)
        goto LongN;
    a = M(EventMusicBuffer);
    a &= 0x08;
    if (z)
        goto MediN;
    a = 0x0f;
    if (!z)
        goto LoadTriCtrlReg;

MediN:
    a = 0x1f;
    if (!z)
        goto LoadTriCtrlReg;

LongN:
    a = 0xff;

LoadTriCtrlReg:
    writeData(SND_TRIANGLE_REG, a);

HandleNoiseMusic:
    a = M(AreaMusicBuffer);
    a &= 0xf3;
    if (z)
        goto ExitMusicHandler;
    --M(Noise_BeatLenCounter);
    if (!z)
        goto ExitMusicHandler;

FetchNoiseBeatData:
    y = M(SoundMemory);
    ++M(SoundMemory);
    a = M(W(MusicDataLow) + y);
    if (!z)
        goto NoiseBeatHandler;
    a = M(NoiseDataLoopbackOfs);
    writeData(SoundMemory, a);
    if (!z)
        goto FetchNoiseBeatData;

NoiseBeatHandler:
    JSR(AlternateLengthHandler, 592);
    writeData(Noise_BeatLenCounter, a);
    a = x;
    a &= 0x3e;
    if (z)
        goto SilentBeat;
    compare(a, 0x30);
    if (z)
        goto LongBeat;
    compare(a, 0x20);
    if (z)
        goto StrongBeat;
    a &= 0x10;
    if (z)
        goto SilentBeat;
    a = 0x1c;
    x = 0x03;
    y = 0x18;
    if (!z)
        goto PlayBeat;

StrongBeat:
    a = 0x1c;
    x = 0x0c;
    y = 0x18;
    if (!z)
        goto PlayBeat;

LongBeat:
    a = 0x1c;
    x = 0x03;
    y = 0x58;
    if (!z)
        goto PlayBeat;

SilentBeat:
    a = 0x10;

PlayBeat:
    writeData(SND_NOISE_REG, a);
    writeData(0x400e, x);
    writeData(0x400f, y);

ExitMusicHandler:
    goto Return;

AlternateLengthHandler:
    x = a;
    a.ror();
    a = x;
    a.rol();
    a.rol();
    a.rol();

ProcessLengthData:
    a &= 0x07;
    c = 0;
    a += M(NoteLenLookupTblOfs);
    a += M(NoteLengthTblAdder);
    y = a;
    a = M(MusicLengthLookupTbl + y);
    goto Return;

LoadControlRegs:
    a = M(EventMusicBuffer);
    a &= 0x08;
    if (z)
        goto NotECstlM;
    a = 0x04;
    if (!z)
        goto AllMus;

NotECstlM:
    a = M(AreaMusicBuffer);
    a &= 0x7d;
    if (z)
        goto WaterMus;
    a = 0x08;
    if (!z)
        goto AllMus;

WaterMus:
    a = 0x28;

AllMus:
    x = 0x82;
    y = 0x7f;
    goto Return;

LoadEnvelopeData:
    a = M(EventMusicBuffer);
    a &= 0x08;
    if (z)
        goto LoadUsualEnvData;
    a = M(EndOfCastleMusicEnvData + y);
    goto Return;

LoadUsualEnvData:
    a = M(AreaMusicBuffer);
    a &= 0x7d;
    if (z)
        goto LoadWaterEventMusEnvData;
    a = M(AreaMusicEnvData + y);
    goto Return;

LoadWaterEventMusEnvData:
    a = M(WaterEventMusEnvData + y);
    goto Return;
    // ;--------------------------------
    // ;music header offsets
    // ;music headers
    // ;header format is as follows:
    // ;1 byte - length byte offset
    // ;2 bytes -  music data address
    // ;1 byte - triangle data offset
    // ;1 byte - square 1 data offset
    // ;1 byte - noise data offset (not used by secondary music)
    // ;--------------------------------
    // ;MUSIC DATA
    // ;square 2/triangle format
    // ;d7 - length byte flag (0-note, 1-length)
    // ;if d7 is set to 0 and d6-d0 is nonzero:
    // ;d6-d0 - note offset in frequency look-up table (must be even)
    // ;if d7 is set to 1:
    // ;d6-d3 - unused
    // ;d2-d0 - length offset in length look-up table
    // ;value of $00 in square 2 data is used as null terminator, affects all sound channels
    // ;value of $00 in triangle data causes routine to skip note
    // ;square 1 format
    // ;d7-d6, d0 - length offset in length look-up table (bit order is d0,d7,d6)
    // ;d5-d1 - note offset in frequency look-up table
    // ;value of $00 in square 1 data is flag alternate control reg data to be loaded
    // ;noise format
    // ;d7-d6, d0 - length offset in length look-up table (bit order is d0,d7,d6)
    // ;d5-d4 - beat type (0 - rest, 1 - short, 2 - strong, 3 - long)
    // ;d3-d1 - unused
    // ;value of $00 in noise data is used as null terminator, affects only noise
    // ;all music data is organized into sections (unless otherwise stated):
    // ;square 2, square 1, triangle, noise
    // ;triangle data used by both sections of third part
    // ;noise data used by lead-in and third part sections
    // ;noise data used by fourth part sections
    // ;square 2 and triangle use the same data, square 1 is unused
    // ;noise data directly follows square 2 here unlike in other songs
    // ;unused space
    // ;-------------------------------------------------------------------------------------
    // ;INTERRUPT VECTORS

// Return handler
// This emulates the RTS instruction using a generated jump table
//
Return:
    switch (popReturnIndex())
    {
    case 0:
        goto Return_0;
    case 1:
        goto Return_1;
    case 2:
        goto Return_2;
    case 3:
        goto Return_3;
    case 4:
        goto Return_4;
    case 5:
        goto Return_5;
    case 6:
        goto Return_6;
    case 7:
        goto Return_7;
    case 8:
        goto Return_8;
    case 9:
        goto Return_9;
    case 10:
        goto Return_10;
    case 11:
        goto Return_11;
    case 12:
        goto Return_12;
    case 13:
        goto Return_13;
    case 14:
        goto Return_14;
    case 15:
        goto Return_15;
    case 16:
        goto Return_16;
    case 17:
        goto Return_17;
    case 18:
        goto Return_18;
    case 19:
        goto Return_19;
    case 20:
        goto Return_20;
    case 21:
        goto Return_21;
    case 22:
        goto Return_22;
    case 23:
        goto Return_23;
    case 24:
        goto Return_24;
    case 25:
        goto Return_25;
    case 26:
        goto Return_26;
    case 27:
        goto Return_27;
    case 28:
        goto Return_28;
    case 29:
        goto Return_29;
    case 30:
        goto Return_30;
    case 31:
        goto Return_31;
    case 32:
        goto Return_32;
    case 33:
        goto Return_33;
    case 34:
        goto Return_34;
    case 35:
        goto Return_35;
    case 36:
        goto Return_36;
    case 37:
        goto Return_37;
    case 38:
        goto Return_38;
    case 39:
        goto Return_39;
    case 40:
        goto Return_40;
    case 41:
        goto Return_41;
    case 42:
        goto Return_42;
    case 43:
        goto Return_43;
    case 44:
        goto Return_44;
    case 45:
        goto Return_45;
    case 46:
        goto Return_46;
    case 47:
        goto Return_47;
    case 48:
        goto Return_48;
    case 49:
        goto Return_49;
    case 50:
        goto Return_50;
    case 51:
        goto Return_51;
    case 52:
        goto Return_52;
    case 53:
        goto Return_53;
    case 54:
        goto Return_54;
    case 55:
        goto Return_55;
    case 56:
        goto Return_56;
    case 57:
        goto Return_57;
    case 58:
        goto Return_58;
    case 59:
        goto Return_59;
    case 60:
        goto Return_60;
    case 61:
        goto Return_61;
    case 62:
        goto Return_62;
    case 63:
        goto Return_63;
    case 64:
        goto Return_64;
    case 65:
        goto Return_65;
    case 66:
        goto Return_66;
    case 67:
        goto Return_67;
    case 68:
        goto Return_68;
    case 69:
        goto Return_69;
    case 70:
        goto Return_70;
    case 71:
        goto Return_71;
    case 72:
        goto Return_72;
    case 73:
        goto Return_73;
    case 74:
        goto Return_74;
    case 75:
        goto Return_75;
    case 76:
        goto Return_76;
    case 77:
        goto Return_77;
    case 78:
        goto Return_78;
    case 79:
        goto Return_79;
    case 80:
        goto Return_80;
    case 81:
        goto Return_81;
    case 82:
        goto Return_82;
    case 83:
        goto Return_83;
    case 84:
        goto Return_84;
    case 85:
        goto Return_85;
    case 86:
        goto Return_86;
    case 87:
        goto Return_87;
    case 88:
        goto Return_88;
    case 89:
        goto Return_89;
    case 90:
        goto Return_90;
    case 91:
        goto Return_91;
    case 92:
        goto Return_92;
    case 93:
        goto Return_93;
    case 94:
        goto Return_94;
    case 95:
        goto Return_95;
    case 96:
        goto Return_96;
    case 97:
        goto Return_97;
    case 98:
        goto Return_98;
    case 99:
        goto Return_99;
    case 100:
        goto Return_100;
    case 101:
        goto Return_101;
    case 102:
        goto Return_102;
    case 103:
        goto Return_103;
    case 104:
        goto Return_104;
    case 105:
        goto Return_105;
    case 106:
        goto Return_106;
    case 107:
        goto Return_107;
    case 108:
        goto Return_108;
    case 109:
        goto Return_109;
    case 110:
        goto Return_110;
    case 111:
        goto Return_111;
    case 112:
        goto Return_112;
    case 113:
        goto Return_113;
    case 114:
        goto Return_114;
    case 115:
        goto Return_115;
    case 116:
        goto Return_116;
    case 117:
        goto Return_117;
    case 118:
        goto Return_118;
    case 119:
        goto Return_119;
    case 120:
        goto Return_120;
    case 121:
        goto Return_121;
    case 122:
        goto Return_122;
    case 123:
        goto Return_123;
    case 124:
        goto Return_124;
    case 125:
        goto Return_125;
    case 126:
        goto Return_126;
    case 127:
        goto Return_127;
    case 128:
        goto Return_128;
    case 129:
        goto Return_129;
    case 130:
        goto Return_130;
    case 131:
        goto Return_131;
    case 132:
        goto Return_132;
    case 133:
        goto Return_133;
    case 134:
        goto Return_134;
    case 135:
        goto Return_135;
    case 136:
        goto Return_136;
    case 137:
        goto Return_137;
    case 138:
        goto Return_138;
    case 139:
        goto Return_139;
    case 140:
        goto Return_140;
    case 141:
        goto Return_141;
    case 142:
        goto Return_142;
    case 143:
        goto Return_143;
    case 144:
        goto Return_144;
    case 145:
        goto Return_145;
    case 146:
        goto Return_146;
    case 147:
        goto Return_147;
    case 148:
        goto Return_148;
    case 149:
        goto Return_149;
    case 150:
        goto Return_150;
    case 151:
        goto Return_151;
    case 152:
        goto Return_152;
    case 153:
        goto Return_153;
    case 154:
        goto Return_154;
    case 155:
        goto Return_155;
    case 156:
        goto Return_156;
    case 157:
        goto Return_157;
    case 158:
        goto Return_158;
    case 159:
        goto Return_159;
    case 160:
        goto Return_160;
    case 161:
        goto Return_161;
    case 162:
        goto Return_162;
    case 163:
        goto Return_163;
    case 164:
        goto Return_164;
    case 165:
        goto Return_165;
    case 166:
        goto Return_166;
    case 167:
        goto Return_167;
    case 168:
        goto Return_168;
    case 169:
        goto Return_169;
    case 170:
        goto Return_170;
    case 171:
        goto Return_171;
    case 172:
        goto Return_172;
    case 173:
        goto Return_173;
    case 174:
        goto Return_174;
    case 175:
        goto Return_175;
    case 176:
        goto Return_176;
    case 177:
        goto Return_177;
    case 178:
        goto Return_178;
    case 179:
        goto Return_179;
    case 180:
        goto Return_180;
    case 181:
        goto Return_181;
    case 182:
        goto Return_182;
    case 183:
        goto Return_183;
    case 184:
        goto Return_184;
    case 185:
        goto Return_185;
    case 186:
        goto Return_186;
    case 187:
        goto Return_187;
    case 188:
        goto Return_188;
    case 189:
        goto Return_189;
    case 190:
        goto Return_190;
    case 191:
        goto Return_191;
    case 192:
        goto Return_192;
    case 193:
        goto Return_193;
    case 194:
        goto Return_194;
    case 195:
        goto Return_195;
    case 196:
        goto Return_196;
    case 197:
        goto Return_197;
    case 198:
        goto Return_198;
    case 199:
        goto Return_199;
    case 200:
        goto Return_200;
    case 201:
        goto Return_201;
    case 202:
        goto Return_202;
    case 203:
        goto Return_203;
    case 204:
        goto Return_204;
    case 205:
        goto Return_205;
    case 206:
        goto Return_206;
    case 207:
        goto Return_207;
    case 208:
        goto Return_208;
    case 209:
        goto Return_209;
    case 210:
        goto Return_210;
    case 211:
        goto Return_211;
    case 212:
        goto Return_212;
    case 213:
        goto Return_213;
    case 214:
        goto Return_214;
    case 215:
        goto Return_215;
    case 216:
        goto Return_216;
    case 217:
        goto Return_217;
    case 218:
        goto Return_218;
    case 219:
        goto Return_219;
    case 220:
        goto Return_220;
    case 221:
        goto Return_221;
    case 222:
        goto Return_222;
    case 223:
        goto Return_223;
    case 224:
        goto Return_224;
    case 225:
        goto Return_225;
    case 226:
        goto Return_226;
    case 227:
        goto Return_227;
    case 228:
        goto Return_228;
    case 229:
        goto Return_229;
    case 230:
        goto Return_230;
    case 231:
        goto Return_231;
    case 232:
        goto Return_232;
    case 233:
        goto Return_233;
    case 234:
        goto Return_234;
    case 235:
        goto Return_235;
    case 236:
        goto Return_236;
    case 237:
        goto Return_237;
    case 238:
        goto Return_238;
    case 239:
        goto Return_239;
    case 240:
        goto Return_240;
    case 241:
        goto Return_241;
    case 242:
        goto Return_242;
    case 243:
        goto Return_243;
    case 244:
        goto Return_244;
    case 245:
        goto Return_245;
    case 246:
        goto Return_246;
    case 247:
        goto Return_247;
    case 248:
        goto Return_248;
    case 249:
        goto Return_249;
    case 250:
        goto Return_250;
    case 251:
        goto Return_251;
    case 252:
        goto Return_252;
    case 253:
        goto Return_253;
    case 254:
        goto Return_254;
    case 255:
        goto Return_255;
    case 256:
        goto Return_256;
    case 257:
        goto Return_257;
    case 258:
        goto Return_258;
    case 259:
        goto Return_259;
    case 260:
        goto Return_260;
    case 261:
        goto Return_261;
    case 262:
        goto Return_262;
    case 263:
        goto Return_263;
    case 264:
        goto Return_264;
    case 265:
        goto Return_265;
    case 266:
        goto Return_266;
    case 267:
        goto Return_267;
    case 268:
        goto Return_268;
    case 269:
        goto Return_269;
    case 270:
        goto Return_270;
    case 271:
        goto Return_271;
    case 272:
        goto Return_272;
    case 273:
        goto Return_273;
    case 274:
        goto Return_274;
    case 275:
        goto Return_275;
    case 276:
        goto Return_276;
    case 277:
        goto Return_277;
    case 278:
        goto Return_278;
    case 279:
        goto Return_279;
    case 280:
        goto Return_280;
    case 281:
        goto Return_281;
    case 282:
        goto Return_282;
    case 283:
        goto Return_283;
    case 284:
        goto Return_284;
    case 285:
        goto Return_285;
    case 286:
        goto Return_286;
    case 287:
        goto Return_287;
    case 288:
        goto Return_288;
    case 289:
        goto Return_289;
    case 290:
        goto Return_290;
    case 291:
        goto Return_291;
    case 292:
        goto Return_292;
    case 293:
        goto Return_293;
    case 294:
        goto Return_294;
    case 295:
        goto Return_295;
    case 296:
        goto Return_296;
    case 297:
        goto Return_297;
    case 298:
        goto Return_298;
    case 299:
        goto Return_299;
    case 300:
        goto Return_300;
    case 301:
        goto Return_301;
    case 302:
        goto Return_302;
    case 303:
        goto Return_303;
    case 304:
        goto Return_304;
    case 305:
        goto Return_305;
    case 306:
        goto Return_306;
    case 307:
        goto Return_307;
    case 308:
        goto Return_308;
    case 309:
        goto Return_309;
    case 310:
        goto Return_310;
    case 311:
        goto Return_311;
    case 312:
        goto Return_312;
    case 313:
        goto Return_313;
    case 314:
        goto Return_314;
    case 315:
        goto Return_315;
    case 316:
        goto Return_316;
    case 317:
        goto Return_317;
    case 318:
        goto Return_318;
    case 319:
        goto Return_319;
    case 320:
        goto Return_320;
    case 321:
        goto Return_321;
    case 322:
        goto Return_322;
    case 323:
        goto Return_323;
    case 324:
        goto Return_324;
    case 325:
        goto Return_325;
    case 326:
        goto Return_326;
    case 327:
        goto Return_327;
    case 328:
        goto Return_328;
    case 329:
        goto Return_329;
    case 330:
        goto Return_330;
    case 331:
        goto Return_331;
    case 332:
        goto Return_332;
    case 333:
        goto Return_333;
    case 334:
        goto Return_334;
    case 335:
        goto Return_335;
    case 336:
        goto Return_336;
    case 337:
        goto Return_337;
    case 338:
        goto Return_338;
    case 339:
        goto Return_339;
    case 340:
        goto Return_340;
    case 341:
        goto Return_341;
    case 342:
        goto Return_342;
    case 343:
        goto Return_343;
    case 344:
        goto Return_344;
    case 345:
        goto Return_345;
    case 346:
        goto Return_346;
    case 347:
        goto Return_347;
    case 348:
        goto Return_348;
    case 349:
        goto Return_349;
    case 350:
        goto Return_350;
    case 351:
        goto Return_351;
    case 352:
        goto Return_352;
    case 353:
        goto Return_353;
    case 354:
        goto Return_354;
    case 355:
        goto Return_355;
    case 356:
        goto Return_356;
    case 357:
        goto Return_357;
    case 358:
        goto Return_358;
    case 359:
        goto Return_359;
    case 360:
        goto Return_360;
    case 361:
        goto Return_361;
    case 362:
        goto Return_362;
    case 363:
        goto Return_363;
    case 364:
        goto Return_364;
    case 365:
        goto Return_365;
    case 366:
        goto Return_366;
    case 367:
        goto Return_367;
    case 368:
        goto Return_368;
    case 369:
        goto Return_369;
    case 370:
        goto Return_370;
    case 371:
        goto Return_371;
    case 372:
        goto Return_372;
    case 373:
        goto Return_373;
    case 374:
        goto Return_374;
    case 375:
        goto Return_375;
    case 376:
        goto Return_376;
    case 377:
        goto Return_377;
    case 378:
        goto Return_378;
    case 379:
        goto Return_379;
    case 380:
        goto Return_380;
    case 381:
        goto Return_381;
    case 382:
        goto Return_382;
    case 383:
        goto Return_383;
    case 384:
        goto Return_384;
    case 385:
        goto Return_385;
    case 386:
        goto Return_386;
    case 387:
        goto Return_387;
    case 388:
        goto Return_388;
    case 389:
        goto Return_389;
    case 390:
        goto Return_390;
    case 391:
        goto Return_391;
    case 392:
        goto Return_392;
    case 393:
        goto Return_393;
    case 394:
        goto Return_394;
    case 395:
        goto Return_395;
    case 396:
        goto Return_396;
    case 397:
        goto Return_397;
    case 398:
        goto Return_398;
    case 399:
        goto Return_399;
    case 400:
        goto Return_400;
    case 401:
        goto Return_401;
    case 402:
        goto Return_402;
    case 403:
        goto Return_403;
    case 404:
        goto Return_404;
    case 405:
        goto Return_405;
    case 406:
        goto Return_406;
    case 407:
        goto Return_407;
    case 408:
        goto Return_408;
    case 409:
        goto Return_409;
    case 410:
        goto Return_410;
    case 411:
        goto Return_411;
    case 412:
        goto Return_412;
    case 413:
        goto Return_413;
    case 414:
        goto Return_414;
    case 415:
        goto Return_415;
    case 416:
        goto Return_416;
    case 417:
        goto Return_417;
    case 418:
        goto Return_418;
    case 419:
        goto Return_419;
    case 420:
        goto Return_420;
    case 421:
        goto Return_421;
    case 422:
        goto Return_422;
    case 423:
        goto Return_423;
    case 424:
        goto Return_424;
    case 425:
        goto Return_425;
    case 426:
        goto Return_426;
    case 427:
        goto Return_427;
    case 428:
        goto Return_428;
    case 429:
        goto Return_429;
    case 430:
        goto Return_430;
    case 431:
        goto Return_431;
    case 432:
        goto Return_432;
    case 433:
        goto Return_433;
    case 434:
        goto Return_434;
    case 435:
        goto Return_435;
    case 436:
        goto Return_436;
    case 437:
        goto Return_437;
    case 438:
        goto Return_438;
    case 439:
        goto Return_439;
    case 440:
        goto Return_440;
    case 441:
        goto Return_441;
    case 442:
        goto Return_442;
    case 443:
        goto Return_443;
    case 444:
        goto Return_444;
    case 445:
        goto Return_445;
    case 446:
        goto Return_446;
    case 447:
        goto Return_447;
    case 448:
        goto Return_448;
    case 449:
        goto Return_449;
    case 450:
        goto Return_450;
    case 451:
        goto Return_451;
    case 452:
        goto Return_452;
    case 453:
        goto Return_453;
    case 454:
        goto Return_454;
    case 455:
        goto Return_455;
    case 456:
        goto Return_456;
    case 457:
        goto Return_457;
    case 458:
        goto Return_458;
    case 459:
        goto Return_459;
    case 460:
        goto Return_460;
    case 461:
        goto Return_461;
    case 462:
        goto Return_462;
    case 463:
        goto Return_463;
    case 464:
        goto Return_464;
    case 465:
        goto Return_465;
    case 466:
        goto Return_466;
    case 467:
        goto Return_467;
    case 468:
        goto Return_468;
    case 469:
        goto Return_469;
    case 470:
        goto Return_470;
    case 471:
        goto Return_471;
    case 472:
        goto Return_472;
    case 473:
        goto Return_473;
    case 474:
        goto Return_474;
    case 475:
        goto Return_475;
    case 476:
        goto Return_476;
    case 477:
        goto Return_477;
    case 478:
        goto Return_478;
    case 479:
        goto Return_479;
    case 480:
        goto Return_480;
    case 481:
        goto Return_481;
    case 482:
        goto Return_482;
    case 483:
        goto Return_483;
    case 484:
        goto Return_484;
    case 485:
        goto Return_485;
    case 486:
        goto Return_486;
    case 487:
        goto Return_487;
    case 488:
        goto Return_488;
    case 489:
        goto Return_489;
    case 490:
        goto Return_490;
    case 491:
        goto Return_491;
    case 492:
        goto Return_492;
    case 493:
        goto Return_493;
    case 494:
        goto Return_494;
    case 495:
        goto Return_495;
    case 496:
        goto Return_496;
    case 497:
        goto Return_497;
    case 498:
        goto Return_498;
    case 499:
        goto Return_499;
    case 500:
        goto Return_500;
    case 501:
        goto Return_501;
    case 502:
        goto Return_502;
    case 503:
        goto Return_503;
    case 504:
        goto Return_504;
    case 505:
        goto Return_505;
    case 506:
        goto Return_506;
    case 507:
        goto Return_507;
    case 508:
        goto Return_508;
    case 509:
        goto Return_509;
    case 510:
        goto Return_510;
    case 511:
        goto Return_511;
    case 512:
        goto Return_512;
    case 513:
        goto Return_513;
    case 514:
        goto Return_514;
    case 515:
        goto Return_515;
    case 516:
        goto Return_516;
    case 517:
        goto Return_517;
    case 518:
        goto Return_518;
    case 519:
        goto Return_519;
    case 520:
        goto Return_520;
    case 521:
        goto Return_521;
    case 522:
        goto Return_522;
    case 523:
        goto Return_523;
    case 524:
        goto Return_524;
    case 525:
        goto Return_525;
    case 526:
        goto Return_526;
    case 527:
        goto Return_527;
    case 528:
        goto Return_528;
    case 529:
        goto Return_529;
    case 530:
        goto Return_530;
    case 531:
        goto Return_531;
    case 532:
        goto Return_532;
    case 533:
        goto Return_533;
    case 534:
        goto Return_534;
    case 535:
        goto Return_535;
    case 536:
        goto Return_536;
    case 537:
        goto Return_537;
    case 538:
        goto Return_538;
    case 539:
        goto Return_539;
    case 540:
        goto Return_540;
    case 541:
        goto Return_541;
    case 542:
        goto Return_542;
    case 543:
        goto Return_543;
    case 544:
        goto Return_544;
    case 545:
        goto Return_545;
    case 546:
        goto Return_546;
    case 547:
        goto Return_547;
    case 548:
        goto Return_548;
    case 549:
        goto Return_549;
    case 550:
        goto Return_550;
    case 551:
        goto Return_551;
    case 552:
        goto Return_552;
    case 553:
        goto Return_553;
    case 554:
        goto Return_554;
    case 555:
        goto Return_555;
    case 556:
        goto Return_556;
    case 557:
        goto Return_557;
    case 558:
        goto Return_558;
    case 559:
        goto Return_559;
    case 560:
        goto Return_560;
    case 561:
        goto Return_561;
    case 562:
        goto Return_562;
    case 563:
        goto Return_563;
    case 564:
        goto Return_564;
    case 565:
        goto Return_565;
    case 566:
        goto Return_566;
    case 567:
        goto Return_567;
    case 568:
        goto Return_568;
    case 569:
        goto Return_569;
    case 570:
        goto Return_570;
    case 571:
        goto Return_571;
    case 572:
        goto Return_572;
    case 573:
        goto Return_573;
    case 574:
        goto Return_574;
    case 575:
        goto Return_575;
    case 576:
        goto Return_576;
    case 577:
        goto Return_577;
    case 578:
        goto Return_578;
    case 579:
        goto Return_579;
    case 580:
        goto Return_580;
    case 581:
        goto Return_581;
    case 582:
        goto Return_582;
    case 583:
        goto Return_583;
    case 584:
        goto Return_584;
    case 585:
        goto Return_585;
    case 586:
        goto Return_586;
    case 587:
        goto Return_587;
    case 588:
        goto Return_588;
    case 589:
        goto Return_589;
    case 590:
        goto Return_590;
    case 591:
        goto Return_591;
    case 592:
        goto Return_592;
    }
}