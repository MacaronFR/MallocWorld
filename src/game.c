#include <game.h>

void moovPlayer(player *player, int ***map) {
	/*
	 if(TOP)
	 	case = checkcase(player->cordinate.x, player->cordinate.y+1)

	 */
}

void fight(player *player, monster *monster, respawn **list, int32_t x, int32_t y, int8_t lvl) {
	int endFight = 0;
	while(endFight == 0) {
		endFight = playerTurnFight(player, monster);
		if(monster->life <= 0)
			endFight = 2;
		else {
			playerTakeDamage(player, monster->strength);
			if(playerIsDead(player))
				endFight = -1;
		}
	}
	switch(endFight) {
		case -1: {
			printc("Vous avez été découpé, brulé, broyé... Quelle mort tragique... :'(", 1, FOREGROUND_RED);
			//endGame();
		}
		case 1: {
			printc("Vous avez fuit le combat ?!  Tapette !!!", 1, FOREGROUND_YELLOW);
		}
		case 2: {
			addMonsterRespawn(monster,list,x,y,lvl);
			playerWinExp(player, monster->exp);
		}
		default: {
			printc("BUG dans la matrice : fight", 1, FOREGROUND_RED);
		}
	}

}

