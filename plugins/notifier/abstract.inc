<?php

/**
 * Email notifier.
 */
class MessageDigest extends MessageNotifierBase {

  public function deliver(array $output = array()) {
    // Do not actually deliver this message because it will be delivered
    // via cron in a digest, but return TRUE to prevent a logged error.
    // Instead, we "deliver" it to the message_digest DB table so that it
    // can be retrieved at a later time.
    $message = $this->message;
    $plugin = $this->plugin;

    $message_digest = array(
      'receiver' => $message->uid,
      'notifier' => $plugin['name'],
      'sent' => FALSE,
      'timestamp' => $message->timestamp,
    );
    if (!empty($message->field_oa_message_space[LANGUAGE_NONE])) {
      $message_digest['gid'] = $message->field_oa_message_space[LANGUAGE_NONE][0]['target_id'];
    }
    // Our $message is a cloned copy of the original $message with the mid field removed to
    // prevent overwriting (this happens in message_subscribe) so we need to fetch the mid manually.
    $mid = db_select('message', 'm')
      ->fields('m', array('mid'))
      ->condition('timestamp', $message->timestamp)
      ->execute()
      ->fetchField();
    if (!empty($mid)) {
      $message_digest['mid'] = $mid;
    }
    drupal_write_record('message_digest', $message_digest);

    return TRUE;
  }

  public function getInterval() {
    return '1 day';
  }

  public function aggregate() {
    $interval = $this->getInterval();
    $start = strtotime('-' . $interval); // Invert $interval since it's in the past.
    $message_groups = array();

    // @TODO: JOIN WITH MESSAGE TABLE, GROUP BY GROUP AND UID.
    $query = db_select('message_digest');
    $query->condition('md.timestamp', $start, '>');
    $query->condition('md.sent', FALSE);
    $query->orderBy('uid');
    $result = $query->execute();

    foreach ($result as $row) {
      $message_groups[$row->uid][] = $row->mid;
    }
    return $message_groups;
  }

  public function format($digest) {
    foreach ($digest as $mid) {
      $message = message_load($mid);
      $build = $message->view();
      $build += array(
        '#theme' => 'message',
        '#entity' => $message,
        '#view_mode' => 'message_notify_email_subject', // @TODO: Default to this but use the Message Digest view mode if it is customized.
      );
      $output = drupal_render($build);
    }
  }

  public function deliverDigest($uid, $message) {
    $account = user_load($uid);
    $lang = !empty($account->language) && $account->language != LANGUAGE_NONE ? $languages[$account->language]: language_default();
    drupal_mail('message_digest', '', $account->mail, $lang);  // @TODO, USE message_notify_send_message() INSTEAD.
  }

}
