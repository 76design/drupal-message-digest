<?php

/**
 * @file
 * Provides a message notifier that stops messages from sending if they're
 * Digest messages, so that they can be aggregated and sent on cron instead.
 */

// Make sure message_notify parent class is included to avoid drush errors
include_once(drupal_get_path('module', 'message_notify') . '/plugins/notifier/email/MessageNotifierEmail.class.php');

/**
 * Email notifier.
 */
class MessageDigest extends MessageNotifierEmail {

  /**
   * Override parent deliver() function.
   */
  public function deliver(array $output = array()) {
    // @TODO, DO WE NEED ANYTHING HERE?
  }
}
