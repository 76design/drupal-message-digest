<?php

/**
 * @file
 * Provides a message notifier that stops messages from sending if they're
 * Digest messages, so that they can be aggregated and sent on cron instead.
 */

$plugin = array(
  'title' => t('Message Digest'),
  'description' => t('Send Message via daily or weekly digests.'),
  'class' => 'MessageDigest',
);

