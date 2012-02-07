<?php

require('html_dom_parser.php');


$levels = array(
  "Study",
  "Challenge"
);

$categories = array(
  "Latin",
  "Arabic",
  "Asian Languages",
  "French",
  "Eponyms",
  "German",
  "Slavic Languages",
  "Dutch",
  "Old English",
  "New World Languages",
  "Japanese",
  "Greek",
  "Italian",
  "Spanish"
);


$dom = new html_dom_parser();
$wdom = new html_dom_parser();


foreach ($levels as $level_id => $level) {
  echo "INSERT INTO levels VALUES ( " . $level_id . ", '" . $level . "' );\n";
}

foreach ($categories as $category_id => $category) {
  echo "INSERT INTO categories VALUES ( " . $category_id . ", '" . $category . "' );\n";
}


foreach ($levels as $level_id => $level) {
  if ($level == "Study") {
    $div_id = 'tab_pane_words_study';
  } else if ($level == "Challenge") {
    $div_id = 'tab_pane_challenge_words';
  } else {
    die("Error:  Unknown level: " . $level);
  }

  foreach ($categories as $category_id => $category) {
    $url = "http://www.myspellit.com/lang_" . strtolower(str_replace(" ", "_", $category)) . ".html";

    get_word_inserts($url, $div_id, $category_id, $level_id);
  }
}


function get_word_inserts($source_uri, $div_id, $category_id, $level_id) {
  global $dom;

  $dom->load_file($source_uri);

  $divs = $dom->find("div#" . $div_id);
  if (count($divs) != 1) {
    die("Error finding div with id=='" . $div_id ."'; expected 1, found " . count($divs));
  }

  foreach ($divs[0]->child as $div_child) {
    if ($div_child->tag == "ol") {
      foreach ($div_child->child as $item) {
	if ($item->tag == "li") {
	  foreach ($item->child as $anchor) {
	    if (($anchor->tag == "a") && ($anchor->target == "_blank")) {
	      $link = $anchor->href;
	      $word = substr($link, strrpos($link, "/") + 1);

	      // Good links are of form:
	      //   http://www.merriam-webster.com/dictionary/impetuous
	      //
	      // Some of the links have the following form (bad?):
	      //   http://www.merriam-webster.com/cgi-bin/dictionary?hw=288634
	      if (strpos($word, "?")) {
		continue;
	      }

	      $wav_uri = get_wav_uri($link, $word);

	      // Audio clips for some of the challenge words are only
	      // available in the for-fee unabridged site; for those
	      // words, $wav_uri comes back as an empty string.
	      if ($wav_uri == '') {
		continue;
	      }

	      echo "INSERT INTO words ( Category_Id, Level_Id, Word, Audio_File ) VALUES ( " . $category_id . ", " . $level_id . ", '" . $word . "', '" . $wav_uri . "' );\n";
	    }
	  }
	}
      }
    }
  }
}

function get_wav_uri($dictionaryUri, &$word) {
  global $wdom;

  $wdom->load_file($dictionaryUri);
  $audiolinks = $wdom->find('a.audio');

  foreach ($audiolinks as $link) {
    // $link->href is of the form:
    //   javascript:popWin('/cgi-bin/audio.pl?transe01.wav=transect')
    //
    // extract the portion between quotes.
    $segments = explode("'", $link->href);
    $path = $segments[1];

    // find the link with qparm == word
    $qparm = substr($path, strrpos($path, '=')+1);
    if ($qparm != $word) {
      continue;
    }

    $url = 'http://www.merriam-webster.com' . $path;

    // Next, get the URI for the .wav file.
    $wdom->load_file($url);
    $result = $wdom->find('embed');

    return $result[0]->src;
  }

  // No matches found (e.g. Latin:condolences).
  // Take the first one and update the word accordingly.
  $link = $audiolinks[0];
  $segments = explode("'", $link->href);
  $path = $segments[1];
  $word = substr($path, strrpos($path, '=')+1);
  $url = 'http://www.merriam-webster.com' . $path;

  // Next, get the URI for the .wav file.
  $wdom->load_file($url);
  $result = $wdom->find('embed');

  return $result[0]->src;
}

?>