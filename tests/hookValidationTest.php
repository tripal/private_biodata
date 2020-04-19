<?php
namespace Tests;

use StatonLab\TripalTestSuite\DBTransaction;
use StatonLab\TripalTestSuite\TripalTestCase;

class hookValidationTest extends TripalTestCase {
  // Uncomment to auto start and rollback db transactions per test method.
  // use DBTransaction;

  /**
   * Tests hook_permission().
   */
  public function testHookPermission() {
    $items = private_biodata_permission();
    $this->assertIsArray($items,
      "hook_permission() should return an array.");

    foreach($items as $perm => $item) {
      $this->assertArrayHasKey('title', $item,
        "Each permission should have a human-readable title but $perm does not.");
      $this->assertArrayHasKey('description', $item,
        "Each permission should have a helpful description but $perm does not.");
    }
  }

  /**
   * Tests hook_schema().
   */
  public function testHookSchema() {
    module_load_include('install', 'private_biodata', 'private_biodata');

    $items = private_biodata_schema();
    $this->assertIsArray($items,
      "hook_schema() should return an array.");

    foreach ($items as $table => $defn) {
      $this->assertArrayHasKey('description', $defn,
        "Each table definition should have a description but $table does not.");
      foreach($defn['fields'] as $field => $item) {
        $this->assertArrayHasKey('type', $item,
          "Each permission should have a type but $table.$field does not.");
        $this->assertArrayHasKey('description', $item,
          "Each permission should have a helpful description but $table.$field does not.");
      }
    }
  }

  /**
   * Tests hook_enable().
   */
  public function testHookEnable() {

    // We want to test the following CVTERM is available
    // as it should have been added during install.
    $cvterm = [
      'id' => 'NCIT:C54104',
      'name' => 'Private',
      'definition' => 'Not public; kept secret or restricted.',
      'cv_id' => ['name' => 'NCIT'],
    ];

    // The module is enabled therefore the term should already be available.
    $result = chado_get_cvterm($cvterm);
    $this->assertNotEmpty($result,
      "NCIT:C54104 (Private) was not available when it should be.");
    $this->assertObjectHasAttribute('cvterm_id', $result,
      "There should only be a single term.");
    $cvterm_id = $result->cvterm_id;

    // Now execute hook_enable directly
    // to check it's adding the exact term we selected.
    private_biodata_enable();
    $result = chado_get_cvterm(['id' => $cvterm['id']]);
    $this->assertNotEmpty($result,
      "NCIT:C54104 (Private) was not available when it should be.");
    $this->assertObjectHasAttribute('cvterm_id', $result,
      "There should only be a single term.");
    $this->assertEquals($cvterm_id, $result->cvterm_id,
      "The hook only inserted a single cvterm and did not duplicate terms.");
  }
}
