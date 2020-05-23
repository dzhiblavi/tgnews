<?php
/*
 * Using:
 *   php dc-check.php <binary> all <port> <source_dir>
 *     where:
 *       <binary>     - path to your tgnews binary
 *       <port>       - any available port
 *       <source_dir> - path to the folder with HTML-files containing article texts
 *
 * You can also check every single command:
 *   php dc-check.php <binary> languages <source_dir>
 *   php dc-check.php <binary> news <source_dir>
 *   php dc-check.php <binary> categories <source_dir>
 *   php dc-check.php <binary> threads <source_dir>
 *   php dc-check.php <binary> server <port> <source_dir>
 *
 * You can change these options if needed:
 *   CURL_VERBOSE    = true|false     - enable/disable cURL verbose information
 *   BINARY_OUTPUT   = true|false     - enable/disable output of your tgnews binary
 *   HTTP_SERVER_LOG = <path_to_file> - path to the file to which stderr/stdout
 *                                      of your tgnews server will be written
 *
 */

define('CURL_VERBOSE',    false);
define('BINARY_OUTPUT',   true);
define('HTTP_SERVER_LOG', 'log.txt');



ini_set('error_reporting', E_ALL & ~E_NOTICE & ~E_STRICT & ~E_DEPRECATED);

list(, $binary, $action, $source_dir) = $argv;

// Checking arguments

if (!$binary || !file_exists($binary) || !is_file($binary)) {
  exitWithError("Binary file not found");
}
if (substr($binary, 0, 1) != '.' && substr($binary, 0, 1) != '/') {
  $binary = './'.$binary;
}
$file_result = shell_exec('file '.escapeshellarg($binary));
list(, $file_result) = explode(': ', $file_result);
$file_result = trim(strtolower($file_result));
//if (strpos($file_result, 'mach-o ') === 0) {
//  exitWithError("Binary file is in unsupported format (MacOS)");
//} elseif (strpos($file_result, 'elf 32-bit ') === 0) {
//  exitWithError("Binary file is in unsupported format (32-bit)");
//}

if ($action == 'server' || $action == 'all') {
  list(, , , $port, $source_dir) = $argv;
  $port_int = intval($port);
  if ($port != $port_int || $port <= 0 || $port > 65535) {
    exitWithError("Invalid port: {$port}");
  }
}

if (!$source_dir || !file_exists($source_dir) || !is_dir($source_dir)) {
  exitWithError("source_dir not found");
}
$source_dir   = rtrim($source_dir, '/');
$source_files = scanDirRecursive($source_dir);
if (empty($source_files)) {
  exitWithError("source_dir is empty");
}

// Running action

switch ($action) {

  case 'languages':
    tgNewsTestLanguages($binary, $source_dir);
    break;

  case 'news':
    tgNewsTestNews($binary, $source_dir);
    break;

  case 'categories':
    tgNewsTestCategories($binary, $source_dir);
    break;

  case 'threads':
    tgNewsTestThreads($binary, $source_dir);
    break;

  case 'server':
    tgNewsTestServer($binary, $port, $source_files);
    break;

  case 'all':
    tgNewsTestLanguages($binary, $source_dir);
    tgNewsTestNews($binary, $source_dir);
    tgNewsTestCategories($binary, $source_dir);
    tgNewsTestThreads($binary, $source_dir);
    tgNewsTestServer($binary, $port, $source_files);
    break;

  default:
    exitWithError("Invalid action: {$action}");
    break;
}
exit;



// Test functions

function tgNewsTestLanguages($binary, $source_dir) {
  debugLog('Running languages command...');
  $exec_command = $binary.' languages '.escapeshellarg($source_dir);
  debugLog($exec_command);
  $exec_result = exec($exec_command, $output_lines, $return_var);
  if ($return_var) {
    exitWithError("Script exit with exit code {$return_var}");
  }
  $output = implode("\n", $output_lines);
  if (!strlen($output)) {
    exitWithError("Script exit with no output");
  }
  $output_data = jsonDecode($output);
  if (!is_array($output_data)) {
    exitWithError("Invalid output format: result should be an array", $output);
  }
  foreach ($output_data as $lang_articles) {
    if (!is_array($lang_articles)) {
      exitWithError("Invalid output format: array item should be an object", $output);
    }
    if (!isset($lang_articles['lang_code'])) {
      exitWithError("Invalid output format: lang_code missed", $output);
    }
    if (!isset($lang_articles['articles'])) {
      exitWithError("Invalid output format: articles missed", $output);
    }
    if (!is_array($lang_articles['articles'])) {
      exitWithError("Invalid output format: articles should be an array", $output);
    }
    foreach ($lang_articles['articles'] as $article) {
      if (strpos($article, '/') !== false) {
        exitWithError("Invalid output format: article should be a file name", $output);
      }
    }
  }
  if (BINARY_OUTPUT) {
    debugLog($output);
  }
  debugLogSuccess('Test completed successfully.');
}

function tgNewsTestNews($binary, $source_dir) {
  debugLog('Running news command...');
  $exec_command = $binary.' news '.escapeshellarg($source_dir);
  debugLog($exec_command);
  $exec_result = exec($exec_command, $output_lines, $return_var);
  if ($return_var) {
    exitWithError("Script exit with exit code {$return_var}");
  }
  $output = implode("\n", $output_lines);
  if (!strlen($output)) {
    exitWithError("Script exit with no output");
  }
  $output_data = jsonDecode($output);
  if (!is_array($output_data)) {
    exitWithError("Invalid output format: result should be an object", $output);
  }
  if (!isset($output_data['articles'])) {
    exitWithError("Invalid output format: articles missed", $output);
  }
  if (!is_array($output_data['articles'])) {
    exitWithError("Invalid output format: articles should be an array", $output);
  }
  foreach ($output_data['articles'] as $article) {
    if (strpos($article, '/') !== false) {
      exitWithError("Invalid output format: article should be a file name", $output);
    }
  }
  if (BINARY_OUTPUT) {
    debugLog($output);
  }
  debugLogSuccess('Test completed successfully.');
}

function tgNewsTestCategories($binary, $source_dir) {
  debugLog('Running categories command...');
  $exec_command = $binary.' categories '.escapeshellarg($source_dir);
  debugLog($exec_command);
  $exec_result = exec($exec_command, $output_lines, $return_var);
  if ($return_var) {
    exitWithError("Script exit with exit code {$return_var}");
  }
  $output = implode("\n", $output_lines);
  if (!strlen($output)) {
    exitWithError("Script exit with no output");
  }
  $output_data = jsonDecode($output);
  if (!is_array($output_data)) {
    exitWithError("Invalid output format: result should be an array", $output);
  }
  foreach ($output_data as $cat_articles) {
    if (!is_array($cat_articles)) {
      exitWithError("Invalid output format: array item should be an object", $output);
    }
    if (!isset($cat_articles['category'])) {
      exitWithError("Invalid output format: category missed", $output);
    }
    if (!isset($cat_articles['articles'])) {
      exitWithError("Invalid output format: articles missed", $output);
    }
    if (!is_array($cat_articles['articles'])) {
      exitWithError("Invalid output format: articles should be an array", $output);
    }
    foreach ($cat_articles['articles'] as $article) {
      if (strpos($article, '/') !== false) {
        exitWithError("Invalid output format: article should be a file name", $output);
      }
    }
  }
  if (BINARY_OUTPUT) {
    debugLog($output);
  }
  debugLogSuccess('Test completed successfully.');
}

function tgNewsTestThreads($binary, $source_dir) {
  debugLog('Running threads command...');
  $exec_command = $binary.' threads '.escapeshellarg($source_dir);
  debugLog($exec_command);
  $exec_result  = exec($exec_command, $output_lines, $return_var);
  if ($return_var) {
    exitWithError("Script exit with exit code {$return_var}");
  }
  $output = implode("\n", $output_lines);
  if (!strlen($output)) {
    exitWithError("Script exit with no output");
  }
  $output_data = jsonDecode($output);
  if (!is_array($output_data)) {
    exitWithError("Invalid output format: result should be an array", $output);
  }
  foreach ($output_data as $thread_articles) {
    if (!is_array($thread_articles)) {
      exitWithError("Invalid output format: array item should be an object", $output);
    }
    if (!isset($thread_articles['title'])) {
      exitWithError("Invalid output format: title missed", $output);
    }
    if (!isset($thread_articles['articles'])) {
      exitWithError("Invalid output format: articles missed", $output);
    }
    if (!is_array($thread_articles['articles'])) {
      exitWithError("Invalid output format: articles should be an array", $output);
    }
    foreach ($thread_articles['articles'] as $article) {
      if (strpos($article, '/') !== false) {
        exitWithError("Invalid output format: article should be a file name", $output);
      }
    }
  }
  if (BINARY_OUTPUT) {
    debugLog($output);
  }
  debugLogSuccess('Test completed successfully.');
}

function tgNewsTestServer($binary, $port, $source_files) {
  $ch  = tgNewsServerInitRequest();
  $pid = tgNewsRunServer($binary, $port);
  tgNewsTestIndexing($ch, $port, $source_files);
  //tgNewsTestRanking($ch, $port);
  tgNewsStopServer($pid);
  $pid = tgNewsRunServer($binary, $port);
  //tgNewsTestRanking($ch, $port);
  tgNewsTestDeleting($ch, $port, $source_files);
  //tgNewsTestRanking($ch, $port);
  tgNewsStopServer($pid);
  debugLogSuccess('Test completed successfully.');
}

function tgNewsRunServer($binary, $port) {
  global $ServerPid;
  debugLog('Running server command...');
  $exec_command = $binary.' server '.escapeshellarg($port);
  debugLog($exec_command);
  $exec_command .= ' > '.escapeshellarg(HTTP_SERVER_LOG).' 2>&1 & echo $!';
  $exec_result   = exec($exec_command, $output_lines, $return_var);
  if ($return_var) {
    exitWithError("Script exit with exit code {$return_var}");
  }
  $pid = intval($exec_result);
  if ($pid <= 0) {
    exitWithError("Invalid pid {$pid}");
  }
  debugLog("HTTP-server running at http://127.0.0.1:{$port}/");
  $ServerPid = $pid;
  return $pid;
}

function tgNewsStopServer($pid) {
  global $ServerPid;
  debugLog('Stopping server...');
  shell_exec('kill -9 '.escapeshellarg($pid));
  $ServerPid = false;
}

function tgNewsTestIndexing($ch, $port, $source_files) {
  foreach ($source_files as $short_filename => $full_filename) {
    $path         = '/'.$short_filename;
    $max_age      = mt_rand(2580000, 2592000);
    $max_age     -= $max_age % 300;
    $file_content = file_get_contents($full_filename);
    $result       = tgNewsServerPerformRequest($ch, $port, 'PUT', $path, $file_content, [
      "Content-Type: text/html",
      "Cache-Control: max-age=".$max_age,
    ]);
    if ($result['http_code'] != 201 && $result['http_code'] != 204) {
      exitWithError("Invalid HTTP code (201 or 204 expected): {$result['http_code']}");
    }
  }
}

function tgNewsTestDeleting($ch, $port, $source_files) {
  $prob = mt_rand(100, 899);
  foreach ($source_files as $short_filename => $full_filename) {
    if (mt_rand(0, 999) < $prob) {
      $path   = '/'.$short_filename;
      $result = tgNewsServerPerformRequest($ch, $port, 'DELETE', $path);
      if ($result['http_code'] != 204 && $result['http_code'] != 404) {
        exitWithError("Invalid HTTP code (204 or 404 expected): {$result['http_code']}");
      }
    }
  }
}

function tgNewsTestRanking($ch, $port) {
  $lang_codes = ['en', 'ru'];
  $categories = ['any', 'society', 'economy', 'technology', 'sports', 'entertainment', 'science', 'other'];
  foreach ($lang_codes as $lang_code) {
    foreach ($categories as $category) {
      $period  = mt_rand(300, 2592000);
      $period -= $period % 300;
      $path    = '/threads?'.http_build_query([
        'period'    => $period,
        'lang_code' => $lang_code,
        'category'  => $category,
      ]);
      $result = tgNewsServerPerformRequest($ch, $port, 'GET', $path);
      if ($result['http_code'] != 200) {
        exitWithError("Invalid HTTP code (200 expected): {$result['http_code']}");
      }
      $output = $result['response'];
      $output_data = jsonDecode($output);
      if (!is_array($output_data)) {
        exitWithError("Invalid output format: result should be an object", $output);
      }
      if (!isset($output_data['threads'])) {
        exitWithError("Invalid output format: threads missed", $output);
      }
      if (!is_array($output_data['threads'])) {
        exitWithError("Invalid output format: threads should be an array", $output);
      }
      foreach ($output_data['threads'] as $thread_articles) {
        if (!is_array($thread_articles)) {
          exitWithError("Invalid output format: thread item should be an object", $output);
        }
        if (!isset($thread_articles['title'])) {
          exitWithError("Invalid output format: title missed", $output);
        }
        if ($category == 'any' && !isset($thread_articles['category'])) {
          exitWithError("Invalid output format: category missed (required for category=any)", $output);
        }
        if (!isset($thread_articles['articles'])) {
          exitWithError("Invalid output format: articles missed", $output);
        }
        if (!is_array($thread_articles['articles'])) {
          exitWithError("Invalid output format: articles should be an array", $output);
        }
        foreach ($thread_articles['articles'] as $article) {
          if (strpos($article, '/') !== false) {
            exitWithError("Invalid output format: article should be a file name", $output);
          }
        }
      }
      if (BINARY_OUTPUT) {
        debugLog($output);
      }
    }
  }
}



// Helper functions

function jsonDecode($json) {
  $error = false;
  try {
    $json_decoded = json_decode($json, true, 512, JSON_THROW_ON_ERROR);
    if ($json_decoded === NULL) {
      $json_error = json_last_error();
      switch ($json_error) {
        case JSON_ERROR_DEPTH:
          $error = "The maximum stack depth has been exceeded";
          break;
        case JSON_ERROR_STATE_MISMATCH:
          $error = "Invalid or malformed JSON";
          break;
        case JSON_ERROR_CTRL_CHAR:
          $error = "Control character error, possibly incorrectly encoded";
          break;
        case JSON_ERROR_SYNTAX:
          $error = "Syntax error";
          break;
        case JSON_ERROR_UTF8:
          $error = "Malformed UTF-8 characters, possibly incorrectly encoded";
          break;
        case JSON_ERROR_RECURSION:
          $error = "One or more recursive references in the value to be encoded";
          break;
        case JSON_ERROR_INF_OR_NAN:
          $error = "One or more NAN or INF values in the value to be encoded";
          break;
        case JSON_ERROR_UNSUPPORTED_TYPE:
          $error = "A value of a type that cannot be encoded was given";
          break;
        case JSON_ERROR_INVALID_PROPERTY_NAME:
          $error = "A property name that cannot be encoded was given";
          break;
        case JSON_ERROR_UTF16:
          $error = "Malformed UTF-16 characters, possibly incorrectly encoded";
          break;
        default:
          if ($json_error !== JSON_ERROR_NONE) {
            $error = "Unknown error";
          }
          break;
      }
    }
  } catch (Exception $e) {
    $error = $e->getMessage();
  }
  if ($error) {
    exitWithError("JSON decoding failed with error: {$error}", $json);
  }
  return $json_decoded;
}

function debugLog() {
  $args = func_get_args();
  echo implode(' ', $args).PHP_EOL;
}

function debugLogSuccess() {
  $args = func_get_args();
  echo "\033[1;32m".implode(' ', $args)."\033[00;0m".PHP_EOL;
}

function debugLogError() {
  $args = func_get_args();
  echo "\033[1;31m".implode(' ', $args)."\033[00;0m".PHP_EOL;
}

function exitWithError($error, $output = false) {
  global $ServerPid;
  debugLogError($error);
  if ($output !== false) {
    debugLog($output);
  }
  if ($ServerPid) {
    tgNewsStopServer($ServerPid);
  }
  exit(1);
}

function scanDirRecursive($dir, $subdir = '') {
  $dir   = rtrim($dir, '/');
  $files = scandir($dir.$subdir);
  $result_files = [];
  foreach ($files as $file) {
    if ($file == '.' || $file == '..') {
      continue;
    }
    $full_filename = $dir.$subdir.'/'.$file;
    if (is_dir($full_filename)) {
      $dir_files = scanDirRecursive($dir, $subdir.'/'.$file);
      foreach ($dir_files as $short_filename => $filename) {
        $result_files[$short_filename] = $filename;
      }
    }
    elseif (substr($file, -5) === '.html') {
      $result_files[$file] = $full_filename;
    }
  }
  return $result_files;
}

function tgNewsServerInitRequest() {
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_HTTP_VERSION,   CURL_HTTP_VERSION_1_1);
  curl_setopt($ch, CURLOPT_VERBOSE,        CURL_VERBOSE);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
  return $ch;
}

function tgNewsServerPerformRequest($ch, $port, $method, $path, $body = '', $http_headers = []) {
  $body_len = strlen($body);
  if ($body_len > 0) {
    $http_headers[] = "Content-Length: {$body_len}";
  }
  $http_headers[] = "Connection: Keep-Alive";
  $http_headers[] = "Keep-Alive: 300";

  curl_setopt($ch, CURLOPT_CUSTOMREQUEST, $method);
  curl_setopt($ch, CURLOPT_URL,           "http://127.0.0.1:{$port}{$path}");
  curl_setopt($ch, CURLOPT_HTTPHEADER,    $http_headers);
  curl_setopt($ch, CURLOPT_POSTFIELDS,    $body);

  debugLog('>', $method, $path);
  $start_time = microtime(true);
  while (true) {
    $response = curl_exec($ch);
    $errno    = curl_errno($ch);
    $error    = curl_error($ch);
    $end_time = microtime(true);
    if ($errno == 7 && $end_time - $start_time < 10) {
      usleep(100000);
      continue;
    }
    if ($errno) {
      exitWithError("CURL request failed with error #{$errno}: {$error}", $response);
    }
    $http_code = curl_getinfo($ch, CURLINFO_HTTP_CODE);
    if ($http_code == 503 && $end_time - $start_time < 300) {
      debugLog('<', $http_code);
      sleep(1);
      debugLog('> Retrying after 1 sec...');
      debugLog('>', $method, $path);
      continue;
    }
    break;
  }

  if ($http_code == 503) {
    exitWithError("Request timed out: 503 Service Unavailable", $response);
  }
  debugLog('<', $http_code);

  return [
    'http_code' => $http_code,
    'response'  => $response,
  ];
}

?>
