

//
// Created by Saqib Bhatti on 17/7/24.
//

#include "Engine.h"


bool Engine::detectCollisions(PlayableCharacter& character) {
    bool reachedGoal = false;

    // Make a rect for all his parts
    FloatRect detectionZone = character.getPosition();

    // Make a FloatRect to test each block
    FloatRect block;

    block.width = TILE_SIZE;
    block.height = TILE_SIZE;

    // Build a zone around thomas to detect collisions
    int startX = (int)(detectionZone.left / TILE_SIZE) - 1;
    int startY = (int)(detectionZone.top / TILE_SIZE) - 1;
    int endX = (int)(detectionZone.left / TILE_SIZE) + 2;
    // Thomas is quite tall so check a few tiles vertically
    int endY = (int)(detectionZone.top / TILE_SIZE) + 3;

    // Make sure we don't test positions lower than zero Or higher than the end of the array
    if (startX < 0) startX = 0;
    if (startY < 0) startY = 0;

    if (endX >= m_LM.getLevelSize().x)
        endX = m_LM.getLevelSize().x;

    if (endY >= m_LM.getLevelSize().y)
        endY = m_LM.getLevelSize().y;

    // Has the character fallen out of the map?
    /*
     * For a character to stop falling, it must collide with a platform. Therefore, if the player
moves out of the map (where there are no platforms), they will continuously fall. The
previous code checks whether the character does not intersect with the FloatRect,
level. If it does not, then it has fallen out of the level and the spawn function sends it
back to the start.
     */
    FloatRect level(0, 0, m_LM.getLevelSize().x * TILE_SIZE,
                    m_LM.getLevelSize().y * TILE_SIZE);

    if (!character.getPosition().intersects(level)) {
        // respawn the character
        character.spawn(m_LM.getStartPosition(), GRAVITY);
    }


    // Loop through all the local blocks
    for (int x = startX; x < endX; x++) {
        for (int y = startY; y < endY; y++) {
            // Initialize the starting position of the current block
            block.left = x * TILE_SIZE;
            block.top = y * TILE_SIZE;

            // Has character been burnt or drowned?
            // Use head as this allows him to sink a bit. If the character's head intersects with
            //one of these tiles, the player is respawned.
            if (m_ArrayLevel[y][x] == 2 || m_ArrayLevel[y][x] == 3) {
                if (character.getHead().intersects(block)) {
                    character.spawn(m_LM.getStartPosition(), GRAVITY);

                    if (m_ArrayLevel[y][x] == 2) {  // fire
                        // Play a sound
                        m_SM.playFallInFire();
                    } else {
                        // Play a sound
                        m_SM.playFallInWater();
                    }
                }
            }

            // Is character colliding with a regular block
            if (m_ArrayLevel[y][x] == 1) {
                if (character.getRight().intersects(block)) {
                    character.stopRight(block.left);
                }
                else if(character.getLeft().intersects(block)) {
                    character.stopLeft(block.left);
                }

                if (character.getFeet().intersects(block)) {
                    character.stopFalling(block.top);
                }
                else if (character.getHead().intersects(block)) {
                    character.stopJump();
                }
            }

            // More collision detection here once we have learned about particle effects

            // Has the character reached the goal?
            if (m_ArrayLevel[y][x] == 4) {
                // Character has reached the goal
                reachedGoal = true;
            }
        }
    }

    // All done, return, whether or not a new level might be required
    return reachedGoal;  // calling code can keep track and respond appropriately if both characters simultaneously
    // reach the goal.
}