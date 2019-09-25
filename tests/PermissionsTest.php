<?php
namespace Tests;

use StatonLab\TripalTestSuite\DBTransaction;
use StatonLab\TripalTestSuite\TripalTestCase;
use Faker\Factory;

class PermissionsTest extends TripalTestCase {
  // Uncomment to auto start and rollback db transactions per test method.
  use DBTransaction;

  /**
   * Test that our new permissions are available.
   * Specifically, `view private [bundle].`
   */
  public function testPermissionsAvailable() {

    $permissions = module_invoke_all('permission');

    // All bundle names are bio_data_##. Content types are created on install
    // of tripal_chado and thus all sites should have them available.
    $bundle_name = db_query('SELECT name FROM tripal_bundle limit 1')->fetchField();

    // First we want to check the permimssion added by this module is available.
    $our_permission = 'view public ' . $bundle_name;
    $this->assertArrayHasKey($our_permission, $permissions,
      "Our permission, $our_permission, was not available.");

    // Next we want to make sure the original permission is still there.
    $originial_permission = 'view ' . $bundle_name;
    $this->assertArrayHasKey($our_permission, $permissions,
      "The original view permission, $originial_permission, was not available.");
  }

  /**
   * Test the permission for a given bundle.
   *
   * NOTE: We only test one bundle since it should be the same for all
   * of them (done in a loop).
   *
   * @group permissions
   */
  public function testPermissionsForUser() {
    $faker = Factory::create();

    // All bundle names are bio_data_##. Content types are created on install
    // of tripal_chado and thus all sites should have them available.
    // @todo create an entity.
    $entity_results = db_query('SELECT id, bundle FROM tripal_entity limit 1')->fetchObject();
    $entity_id = $entity_results->id;
    $bundle_name = $entity_results->bundle;

    $permission_name = "view public $bundle_name";

    // All permissions are assigned to users via roles...
    // Thus, create two new roles:
    // 1) A role which cannot use any of the permissions.
    $role_canNOT = new \stdClass();
    $role_canNOT->name = $faker->name();
    user_role_save($role_canNOT);
    // 2) A role which can use all of them.
    $role_can = new \stdClass();
    $role_can->name = $faker->name();
    user_role_save($role_can);
    user_role_grant_permissions($role_can->rid, [$permission_name]);

    // Create our users:
    // 1) a user without tripal permissions but who is still authenticated.
    $email = $faker->email();
    $user_canNOT = array(
      'name' => $faker->name(),
      'pass' => $faker->password(), // note: do not md5 the password
      'mail' => $email,
      'status' => 1,
      'init' => $email,
      'roles' => array(
        DRUPAL_AUTHENTICATED_RID => 'authenticated user',
        $role_canNOT->rid => $role_canNOT->name,
      ),
    );
    $user_canNOT = user_save('', $user_canNOT); // 1st param blank so new user is created.
    $user_canNOT_uid = $user_canNOT->uid;
    // 2) A user with the role giving them all tripal permissions.
    $email = $faker->email();
    $user_can = array(
      'name' => $faker->name(),
      'pass' => $faker->password(), // note: do not md5 the password
      'mail' => $email,
      'status' => 1,
      'init' => $email,
      'roles' => array(
        DRUPAL_AUTHENTICATED_RID => 'authenticated user',
        $role_can->rid => $role_can->name,
      ),
    );
    $user_can = user_save('', $user_can); // 1st param blank so new user is created.
    $user_can_uid = $user_can->uid;

    $entity_load = entity_load('TripalEntity', [$entity_id]);
    $entity = $entity_load[$entity_id];

    // Now we need to clear the user_access cache and re-load our users
    // in order to see our newly assigned roles and permissions reflected.
    drupal_static_reset('user_access');
    unset($user_can, $user_canNOT);
    $user_can = user_load($user_can_uid, TRUE);
    $user_canNOT = user_load($user_canNOT_uid, TRUE);

    // Check that our roles were assigned this permission correctly.
    $all_roles_with_permission = user_roles(TRUE, $permission_name);
    $this->assertArrayHasKey($role_can->rid, $all_roles_with_permission,
      "Our newly created role doesn't have the expected permission.");
    $this->assertArrayNotHasKey($role_canNOT->rid, $all_roles_with_permission,
      "The roles that shouldn't have the permission, does?");

    // Check that the user who should be able to access the content, can.
    $result = tripal_entity_access('view', $entity, $user_can);
    $this->assertTrue($result,
      "The current user does not have permission to view the public entity.");

    // Check that the user who should NOT be able to access the content, can NOT.
    // Note we can only check if this permission is not given to the authenticated user.
    $has_authenticated = in_array(
      'authenticated user',
      $all_roles_with_permission
    );
    if ($has_authenticated == FALSE) {
      $result = tripal_entity_access($op, $entity, $user_canNOT);
      $this->assertFalse($result,
        "The current user does but shouldn't have permission to view the public entity.");
    }
  }
}
