<?php

namespace Drupal\quicktabs\Plugin\TabType;

use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Ajax\AjaxResponse;
use Drupal\Core\Ajax\ReplaceCommand;
use Drupal\Core\StringTranslation\StringTranslationTrait;
use Drupal\quicktabs\TabTypeBase;
use Drupal\Core\Entity\EntityTypeManagerInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides a 'block content' tab type.
 *
 * @TabType(
 *   id = "block_content",
 *   name = @Translation("block"),
 * )
 */
class BlockContent extends TabTypeBase {

  use StringTranslationTrait;

  /**
   * The entity type manager.
   *
   * @var \Drupal\Core\Entity\EntityTypeManagerInterface
   */
  protected $entityTypeManager;

  /**
   * Creates a BlockComponentRenderArray object.
   *
   * @param array $configuration
   *   A configuration array containing information about the plugin instance.
   * @param string $plugin_id
   *   The plugin ID for the plugin instance.
   * @param mixed $plugin_definition
   *   The plugin implementation definition.
   * @param \Drupal\Core\Entity\EntityTypeManagerInterface $entity_type_manager
   *   The entity type manager.
   */
  public function __construct(array $configuration, $plugin_id, $plugin_definition, EntityTypeManagerInterface $entity_type_manager) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);

    $this->entityTypeManager = $entity_type_manager;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $configuration,
      $plugin_id,
      $plugin_definition,
      $container->get('entity_type.manager')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function optionsForm(array $tab) {
    $plugin_id = $this->getPluginDefinition()['id'];
    $form = [];
    $form['bid'] = [
      '#type' => 'select',
      '#options' => $this->getBlockOptions(),
      '#default_value' => isset($tab['content'][$plugin_id]['options']['bid']) ? $tab['content'][$plugin_id]['options']['bid'] : '',
      '#title' => $this->t('Select a block'),
      '#ajax' => [
        'callback' => [$this, 'blockTitleAjaxCallback'],
        'event' => 'change',
        'progress' => [
          'type' => 'throbber',
          'message' => 'Please wait...',
        ],
        'effect' => 'fade',
      ],
    ];
    $form['block_title'] = [
      '#type' => 'textfield',
      '#default_value' => isset($tab['content'][$plugin_id]['options']['block_title']) ? $tab['content'][$plugin_id]['options']['block_title'] : '',
      '#title' => $this->t('Block Title'),
      '#prefix' => '<div id="block-title-textfield-' . $tab['delta'] . '">',
      '#suffix' => '</div>',
    ];
    $form['display_title'] = [
      '#type' => 'checkbox',
      '#title' => $this->t('Display block title'),
      '#default_value' => isset($tab['content'][$plugin_id]['options']['display_title']) ? $tab['content'][$plugin_id]['options']['display_title'] : 0,
    ];
    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function render(array $tab) {
    $options = $tab['content'][$tab['type']]['options'];

    if (strpos($options['bid'], 'block_content') !== FALSE) {
      $parts = explode(':', $options['bid']);
      $block = $this->entityTypeManager->loadEntityByUuid($parts[0], $parts[1]);
      $block_content = BlockContent::load($block->id());
      $render = $this->entityTypeManager->getViewBuilder('block_content')->view($block_content);
    }
    else {
      $block_manager = \Drupal::service('plugin.manager.block');
      // You can hard code configuration or you load from settings.
      $config = [];
      $plugin_block = $block_manager->createInstance($options['bid'], $config);

      // Some blocks might implement access check.
      $access_result = $plugin_block->access(\Drupal::currentUser(), TRUE);
      // Return empty render array if user doesn't have access.
      if ($access_result->isForbidden()) {
        return [];
      }

      $render = $plugin_block->build();
    }

    return $render;
  }

  /**
   * Get options for the block.
   */
  private function getBlockOptions() {
    $block_manager = \Drupal::service('plugin.manager.block');
    $context_repository = \Drupal::service('context.repository');

    // Only add blocks which work without any available context.
    $definitions = $block_manager->getDefinitionsForContexts($context_repository->getAvailableContexts());
    // Order by category, and then by admin label.
    $definitions = $block_manager->getSortedDefinitions($definitions);

    $blocks = [];
    foreach ($definitions as $block_id => $definition) {
      $blocks[$block_id] = $definition['admin_label'] . ' (' . $definition['provider'] . ')';
    }

    return $blocks;
  }

  /**
   * Ajax callback to change block title when block is selected.
   */
  public function blockTitleAjaxCallback(array &$form, FormStateInterface $form_state) {
    $tab_index = $form_state->getTriggeringElement()['#array_parents'][2];
    $element_id = '#block-title-textfield-' . $tab_index;
    $selected_block = $form_state->getValue('configuration_data')[$tab_index]['content']['block_content']['options']['bid'];

    $block_manager = \Drupal::service('plugin.manager.block');
    $context_repository = \Drupal::service('context.repository');
    $definitions = $block_manager->getDefinitionsForContexts($context_repository->getAvailableContexts());

    $form['block_title'] = [
      '#type' => 'textfield',
      '#value' => $definitions[$selected_block]['admin_label'],
      '#title' => $this->t('Block Title'),
      '#prefix' => '<div id="block-title-textfield-' . $tab_index . '">',
      '#suffix' => '</div>',
    ];

    $form_state->setRebuild(TRUE);
    $ajax_response = new AjaxResponse();
    $ajax_response->addCommand(new ReplaceCommand($element_id, $form['block_title']));

    return $ajax_response;
  }

}
