#include <game.h>


void fight(player *player, monster *monster, respawn **list, int32_t x, int32_t y) {
	int endFight = 0;
	while(endFight == 0) {
		endFight = playerTurnFight(player, monster);
		if(monster->life <= 0)
			endFight = 1;
		else {
			playerTakeDamage(player, monster->strength);
			if(playerIsDead(player))
				endFight = -1;
		}
	}

}

