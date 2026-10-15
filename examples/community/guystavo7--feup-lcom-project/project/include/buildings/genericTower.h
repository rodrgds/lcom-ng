#ifndef GENERIC_TOWER
#define GENERIC_TOWER

/** @file genericTower.h
 *  @brief Torre generica: sprites base/arqueiro, alcance, cooldown, projéteis e callback de update.
 */

#include "../../../lab5/terminal.h"
#include "../../../lab5/spriteXMP.h"
#include "../components/animatedSprite.h"
#include "../components/projectile.h"
#include "../utils/vec2d.h"
#include "../arena.h"

#include <stdbool.h>

typedef struct GenericTower GenericTower;

/** @brief Tipo de torre (stats e projectil por omissao). */
typedef enum {
    TOWER_KIND_BASIC,
    TOWER_KIND_FAST,
    TOWER_KIND_PIERCING,
    TOWER_KIND_MORTAR,
    TOWER_KIND_ELETRICT,
	TOWER_KIND_LONG,
} TowerKind;

/** @brief Constroi torre com geometria, alcance, callback de logica e tipo de projectil.
 *  @param[in] upperHalf Sprite da metade superior da base.
 *  @param[in] lowerHalf Sprite da metade inferior.
 *  @param[in] archerIdle Animacao idle do atirador.
 *  @param[in] archerShoot Animacao de disparo.
 *  @param[in] position Posicao da torre no mapa.
 *  @param[in] size Tamanho da torre (desenho/hitbox visual).
 *  @param[in] hitBoxOffset Offset da hitbox relativamente a position.
 *  @param[in] rangeSize Tamanho da area de deteccao de alvos.
 *  @param[in] rangeOffset Offset da area de alcance.
 *  @param[in] towerUpdate Callback especifico do tipo de torre (NULL usa generico).
 *  @param[in] cooldown Segundos entre disparos.
 *  @param[in] projectileType Tipo de Projectile a criar.
 *  @param[in] arrowSprites Sprites dos projéteis / setas.
 *  @param[in] n Numero de sprites em arrowSprites.
 *  @param[in] kind Identificador TowerKind.
 *  @return Torre alocada.
 */
GenericTower* towerNew(SpriteXPM* upperHalf, SpriteXPM* lowerHalf, AnimatedSprite* archerIdle, AnimatedSprite* archerShoot, Vec2d position, Vec2d size, Vec2d hitBoxOffset, Vec2d rangeSize, Vec2d rangeOffset, void (*towerUpdate)(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime), float cooldown, ProjectileType projectileType, SpriteXPM** arrowSprites, int n, TowerKind kind);

/** @brief Liberta torre e projéteis activos.
 *  @param[in] tower Torre.
 */
void towerFree(GenericTower* tower);

/** @brief Hitbox da estrutura da torre (nao confundir com range).
 *  @param[in] tower Torre.
 *  @return BoxCollider.
 */
BoxCollider* towerGetHitBox(GenericTower* tower);

/** @brief Posicao logica da torre.
 *  @param[in] tower Torre.
 *  @return Vec2d posicao.
 */
Vec2d towerGetPosition(GenericTower* tower);

/** @brief Tipo enumerado da torre.
 *  @param[in] tower Torre.
 *  @return TowerKind.
 */
TowerKind towerGetKind(GenericTower* tower);

/** @brief true se torre basica.
 *  @param[in] tower Torre.
 *  @return Comparacao de kind.
 */
bool towerIsBasic(GenericTower* tower);

/** @brief true se torre de alcance longo.
 *  @param[in] tower Torre.
 *  @return Comparacao de kind.
 */
bool towerIsLong(GenericTower* tower);

/** @brief Actualiza IA de disparo, alvo e projéteis.
 *  @param[in,out] tower Torre.
 *  @param[in] terminal Terminal.
 *  @param[in,out] arena Arena (monstros, colisoes).
 *  @param[in] deltaTime Delta em segundos.
 */
void towerUpdate(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime);

/** @brief Desenha apenas a metade superior (base) por cima de outras camadas se necessario.
 *  @param[in,out] tower Torre.
 *  @param[in] terminal Terminal.
 *  @param[in,out] arena Arena.
 *  @param[in] deltaTime Delta em segundos.
 */
void towerDrawUpperHalf(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime);

/** @brief Adiciona projectil activo ao array interno da torre.
 *  @param[in,out] tower Torre.
 *  @param[in] projectile Projectil ja criado.
 *  @return true se couber na capacidade.
 */
bool towerAddProjectile(GenericTower* tower, Projectile* projectile);

/** @brief Numero de projéteis activos geridos pela torre.
 *  @param[in] tower Torre.
 *  @return Contagem.
 */
int towerGetProjectilesCount(GenericTower* tower);

/** @brief Acesso indexado a um projectil.
 *  @param[in] tower Torre.
 *  @param[in] index Indice valido.
 *  @return Ponteiro Projectile.
 */
Projectile* towerGetProjectile(GenericTower* tower, int index);

#endif
