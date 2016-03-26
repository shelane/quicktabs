<?php
/**
 * @file
 * Contains \Drupal\quicktabs\Form\QuicktabCloneForm.php
 */
namespace Drupal\quicktabs\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * Class QuicktabCloneForm
 *
 */
class QuicktabCloneForm extends FormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'quicktab clone';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state ){
    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {

  }
}
