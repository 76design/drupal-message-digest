<?php

/**
 * Email notifier.
 */
class MessageDigest extends MessageNotifierBase {

  public function deliver(array $output = array()) {
    // Do not deliver this message because it will be delivered via cron in a digest.
    return TRUE;
  }

  public function aggregate($interval = '1 day') {
    $start = strtotime(-$interval);
    $message_groups = array();

    $query = new EntityFieldQuery();
    $query = db_select('message', 'm');
    $query->fields('m');
    $query->condition('timestamp', $start, '>');
    $query->orderBy('uid');
    $result = $query->execute();

    foreach ($result as $row) {
      $message_groups[$row->uid][] = $row->mid;
    }
    return $message_groups;
  }

  public function format($digest) {
    foreach ($digest as $mid) {
      $entity = entity_load('message', array($mid));
      $message = $entity[$row['mid']];
      $wrapper = entity_metadata_wrapper('message', $message);
      $message = message_create('task_notification_message', array('uid' => $metadata['uid']));
      // @TODO: MAKE THIS WORK.
    }
  }

}
