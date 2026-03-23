//
// Created by leon on 22.03.26.
//

#include "bulletspawner.h"

#include "bullet.h"

using namespace Engine;

BulletSpawner::BulletSpawner() : m_fBulletSpeed(1000) {
}

void BulletSpawner::Init() {
    Node::Init();
    m_name = "BulletSpawner";
}

void BulletSpawner::ShootInDirection(Vector2d dir) {

    Bullet* newBullet = new Bullet(1000, 1, Vector2d(dir), m_globalTransform.position);
    AddChild(*newBullet);
}
