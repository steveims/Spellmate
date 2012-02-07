<?
/*******************************************************************************
Authors: S. C. Chen (me578022@gmail.com)
Origional: Jose Solorzano (https://sourceforge.net/projects/php-html/)
Licensed under The MIT License
Redistributions of files must retain the above copyright notice.
*******************************************************************************/

define('HDOM_TYPE_ELEMENT', 1);
define('HDOM_TYPE_TEXT',    3);
define('HDOM_TYPE_ENDTAG',  4);
define('HDOM_QUOTE_DOUBLE', 0);
define('HDOM_QUOTE_SINGLE', 1);
define('HDOM_QUOTE_NO',     3);
define('HDOM_INFO_BEGIN',   0);
define('HDOM_INFO_END',     1);
define('HDOM_INFO_SLASH',   2);
define('HDOM_INFO_QUOTE',   3);
define('HDOM_INFO_SPACE',   4);
define('HDOM_INFO_TEXT',    5);
define('HDOM_INFO_INNER',   6);
define('HDOM_INFO_OUTER',   7);

// quick function
// -----------------------------------------------------------------------------
// get dom form file
function file_get_dom($filepath, $lowercase=true) {
    $dom = new html_dom_parser;
    $dom->load_file($filepath, $lowercase);
    return $dom;
}

// write dom to file
function file_put_dom($filepath, $dom) {
    return $dom->save_file($filepath);
}

// get dom form string
function str_get_dom($str, $lowercase=true) {
    $dom = new html_dom_parser;
    $dom->load($str, $lowercase);
    return $dom;
}

// write dom to string
function str_put_dom($dom) {
    return $dom->save();
}

// html dom node
// -----------------------------------------------------------------------------
class html_dom_node {
    public $nodetype;
    public $tag;
    public $attr = array();
    public $parent = null;
    public $child = array();
    public $parser = null;
    public $info = array(
        HDOM_INFO_BEGIN=>0, 
        HDOM_INFO_END=>0, 
        HDOM_INFO_SLASH=>false, 
        HDOM_INFO_QUOTE=>array(), 
        HDOM_INFO_SPACE=>array(),
        HDOM_INFO_TEXT=>'', 
    );

    function __construct($parser) {
        $this->parser = $parser;
    }

    function __get($var) {
        if (isset($this->attr[$var])) return $this->attr[$var];
        if ($var=='innertext') return $this->innertext();
        if ($var=='outertext') return $this->outertext();
    }

    function __set($var, $val) {
        if ($var=='innertext') return $this->info[HDOM_INFO_INNER] = $val;
        if ($var=='outertext') return $this->info[HDOM_INFO_OUTER] = $val;
        if (!isset($this->attr[$var])) {
            $count = count($this->info[HDOM_INFO_SPACE]);
            $this->info[HDOM_INFO_SPACE][$count-2] = ' ';
            $this->info[HDOM_INFO_QUOTE][] = HDOM_QUOTE_DOUBLE;
        }
        $this->attr[$var] = $val;
    }

    function __isset($var) {
        if ($var=='innertext') return true;
        if ($var=='outertext') return true;
        return isset($this->attr[$var]);
    }

    // clean up memory due to php5 circular references memory leak...
    function clear() {
        //$this->parser = null;
        $this->parent = null;
        $this->child = null;
    }

    // get dom node inner html
    function innertext() {
        if (isset($this->info[HDOM_INFO_INNER])) return $this->info[HDOM_INFO_INNER];
        if ($this->info[HDOM_INFO_BEGIN]==$this->info[HDOM_INFO_END]) return $this->text();

        $ret = '';
        for($i=$this->info[HDOM_INFO_BEGIN]+1; $i<$this->info[HDOM_INFO_END]; ++$i)
            $ret .= $this->parser->nodes[$i]->text();
        return $ret;
    }

    // get dom node outer html (with tag)
    function outertext() {
        if (isset($this->info[HDOM_INFO_OUTER])) return $this->info[HDOM_INFO_OUTER];
        if ($this->info[HDOM_INFO_BEGIN]==$this->info[HDOM_INFO_END]) return $this->text();

        $ret = '';
        $last = $this->info[HDOM_INFO_END]+1;
        for($i=$this->info[HDOM_INFO_BEGIN]; $i<$last; ++$i)
            $ret .= $this->parser->nodes[$i]->text();
        return $ret;
    }

    // get node text
    function text() {
        if ($this->nodetype==HDOM_TYPE_TEXT) return $this->info[HDOM_INFO_TEXT];
        if ($this->nodetype==HDOM_TYPE_ENDTAG) return '</'.$this->tag.'>';

        $ret = '<'.$this->tag;
        $i = 0;
        $j = 0;
        $count_space = count($this->info[HDOM_INFO_SPACE]);
        foreach($this->attr as $key=>$val) {
            $ret .= ($j<$count_space) ? $this->info[HDOM_INFO_SPACE][$j++] : ' ';

            //no value attr: nowrap, checked selected...
            if ($val===null) {
                $ret .= $key;
                if ($j<$count_space) ++$j;
            }
            else {
                $quote = '"';
                if ($this->info[HDOM_INFO_QUOTE][$i]==HDOM_QUOTE_DOUBLE) $quote = '"';
                else if ($this->info[HDOM_INFO_QUOTE][$i]==HDOM_QUOTE_SINGLE) $quote = "'";
                else $quote = '';

                $ret .= $key;
                if ($j<$count_space) $ret .= $this->info[HDOM_INFO_SPACE][$j++];

                $ret .= "=";
                if ($j<$count_space) $ret .= $this->info[HDOM_INFO_SPACE][$j++];

                $ret .= $quote.$val.$quote;
            }
            ++$i;
        }
        
        if ($j<$count_space) $ret .= $this->info[HDOM_INFO_SPACE][$j];
        if($this->info[HDOM_INFO_SLASH]) $ret .= '/';
        return $ret.'>';
    }
}

// html dom parser
// -----------------------------------------------------------------------------
class html_dom_parser {
    public 	$nodes = array();
    public  $root = null;
    private $parent = null;
    private $lowercase = false;
    private $pos;
    private $char;
    private $size;
    private $html;
    private $index;
    private $noise = array();
    private $token_blank = array(' '=>1, "\t"=>1, "\r"=>1, "\n"=>1);
    private $token_equal = array(' '=>1, '='=>1, '/'=>1, '>'=>1, '<'=>1, "\t"=>1, "\r"=>1, "\n"=>1);
    private $token_slash = array(' '=>1, '/'=>1, '>'=>1, "\t"=>1, "\r"=>1, "\n"=>1);
    private $token_attr  = array(' '=>1, '>'=>1, "\t"=>1, "\r"=>1, "\n"=>1);
    private $tag_self_closing = array('img'=>1, 'br'=>1, 'input'=>1, 'meta'=>1, 'link'=>1, 'hr'=>1);

    // load html from string
    function load($str, $lowercase=true) {
        $this->clear();

        $this->html = $str;
        $this->lowercase = $lowercase;
        $this->index = 0;
        $this->root = new html_dom_node($this);
        $this->parent = $this->root;

        // remove comment
        $this->remove_noise("'<!--(.*?)-->'is");
        // remove css style
        $this->remove_noise("'<\s*style[^>]*?>(.*?)<\s*/\s*style\s*>'is", false);
        // remove javascript
        $this->remove_noise("'<\s*script[^>]*?>(.*?)<\s*/\s*script\s*>'is", false);
        // remove server side script
        $this->remove_noise("'(<\?)(.*?)(\?>)'is");
        //$this->remove_noise("'(<\%)(.*?)(\%>)'is");

        // parse
        $this->size = strlen($this->html);
        $this->pos = 0;
        if ($this->size>0) $this->char = $this->html[0];
        while ($this->parse()!=false);
    }

    // load html from file
    function load_file($filepath, $lowercase=true) {
        $this->load(file_get_contents($filepath), $lowercase);
    }

    // save dom as string
    function save() {
        $ret = '';
        $count = count($this->nodes);
        for ($i=0; $i<$count; ++$i) {
            // outertext defined
            if (isset($this->nodes[$i]->info[HDOM_INFO_OUTER])) {
                $ret .= $this->nodes[$i]->info[HDOM_INFO_OUTER];
                if ($this->nodes[$i]->info[HDOM_INFO_END]>0)
                    $i = $this->nodes[$i]->info[HDOM_INFO_END];
                continue;
            }

            $ret .= $this->nodes[$i]->text();

            // innertext defined
            if (isset($this->nodes[$i]->info[HDOM_INFO_INNER]) && $this->nodes[$i]->info[HDOM_INFO_END]>0) {
                $ret .= $this->nodes[$i]->info[HDOM_INFO_INNER];
                $i = $this->nodes[$i]->info[HDOM_INFO_END]-1;
            }
        }
        return $ret;
    }

    // save dom string to file
    function save_file($filepath) {
        return file_put_contents($filepath, $this->save());
    }

    // find dom node by css selector
    function find($selector) {
        $key = null;
        $val = null;
        $tag = null;

        $selector = trim($selector);
        if ($selector=='*')
            return $this->nodes;

        if (($pos1=strpos($selector, '['))!==false && ($pos2=strrpos($selector, ']'))!==false) {
            $attr = split('=', substr($selector, $pos1+1, $pos2-$pos1-1));
            $key = $attr[0];
            if(isset($key[0]) && $key[0]=='@') $key = substr($key, 1);
            if (isset($attr[1])) $val = $attr[1];
            $tag = substr($selector, 0, $pos1);
        }
        else if (($pos=strpos($selector, '#'))!==false) {
            $key = 'id';
            $val = substr($selector, $pos+1);
            $tag = substr($selector, 0, $pos);
        }
        else if (($pos=strpos($selector, '.'))!==false) {
            $key = 'class';
            $val = substr($selector, $pos+1);
            $tag = substr($selector, 0, $pos);
        } else
            $tag = $selector;

        if ($this->lowercase) {
            if ($tag) $tag = strtolower($tag);
            if ($key) $key = strtolower($key);
        }

        $ret = array();
        foreach($this->nodes as &$n) {
            if ($n->nodetype==HDOM_TYPE_ENDTAG) continue;
            $pass = true;
            if ($tag && $tag!=$n->tag) $pass = false;
            if ($pass && $key && !(isset($n->attr[$key]))) $pass = false;
            if ($pass && $key && $val && !(isset($n->attr[$key]) && $n->attr[$key]==$val)) $pass = false;
            if ($pass) $ret[] = $n;
        }
        return $ret;
    }

    // clean up memory due to php5 circular references memory leak...
    function clear() {
        $this->html = '';
        $this->noise = array();
        $this->parent->parent = null;
        $this->parent->child = null;
        $this->parent = null;

        if ($this->root) $this->root->clear();
        $this->root = null;

        foreach($this->nodes as $n) {
            $n->clear();
            $n = null;
        }
        $this->nodes = array();
    }

    // parse html content
    private function parse() {
        $s = $this->copy_until_char('<', false);
        if ($s=='') return $this->read_tag();

        $node = new html_dom_node($this);
        $this->nodes[] = $node;
        $node->info[HDOM_INFO_BEGIN] = $this->index;
        $node->info[HDOM_INFO_END] = $this->index;
        ++$this->index;

        $node->nodetype = HDOM_TYPE_TEXT;
        $node->tag = 'text';
        $node->info[HDOM_INFO_TEXT] = $this->restore_noise($s);
        $node->parent = $this->parent;
        $this->parent->child[] = $node;
        return true;
    }

    // read tag info
    private function read_tag() {
        if ($this->char!='<') return false;

        // next 
        $this->char = $this->html[++$this->pos];
        $this->skip($this->token_blank);

        $node = new html_dom_node($this);
        $this->nodes[] = $node;
        $node->info[HDOM_INFO_BEGIN] = $this->index;
        ++$this->index;

        // end tag
        if ($this->char=='/') {
            // next
            $this->char = $this->html[++$this->pos];
            $this->skip($this->token_blank);
            $node->nodetype = HDOM_TYPE_ENDTAG;
            $node->tag = $this->copy_until_char('>');
            if ($this->lowercase) $node->tag = strtolower($node->tag);
            
            // next
            if(++$this->pos<$this->size) $this->char = $this->html[$this->pos];

            $this->parent->info[HDOM_INFO_END] = $this->index-1;
            if (isset($this->parent->parent)) $this->parent = $this->parent->parent;
            $node->parent = $this->parent;
            return true;
        }

        $node->tag = $this->copy_until($this->token_slash);
        $node->parent = $this->parent;
        $this->parent->child[] = $node;

        // text
        if (!preg_match("/^[A-Za-z0-9_\\-]+$/", $node->tag)) {
            $node->nodetype = HDOM_TYPE_TEXT;
            $node->info[HDOM_INFO_END] = $this->index-1;
            $node->info[HDOM_INFO_TEXT] = '<' . $node->tag . $this->copy_until_char('>') . '>';
            $node->tag = 'text';
            // next
            if(++$this->pos<$this->size) $this->char = $this->html[$this->pos];
            return true;
        }

        // begin tag
        $node->nodetype = HDOM_TYPE_ELEMENT;
        if ($this->lowercase) $node->tag = strtolower($node->tag);

        // attributes
        while(($node->info[HDOM_INFO_SPACE][]=$this->copy_skip($this->token_blank))!='' || ($this->char!='>' && $this->char!='/')) {
            $name = $this->copy_until($this->token_equal);

            if ($name!='/' && $name!='') {
                $node->info[HDOM_INFO_SPACE][] = $this->copy_skip($this->token_blank);
                if ($this->lowercase) $name = strtolower($name);
                
                if ($this->char=='=') {
                    // next
                    $this->char = $this->html[++$this->pos];
                    $this->parse_attr($node, $name);
                }
                else {
                    //no value attr: nowrap, checked selected...
                    $node->attr[$name] = null;
                    $node->info[HDOM_INFO_QUOTE][] = HDOM_QUOTE_NO;
                    if ($this->char!=='>') {
                        // prev
                        $this->char = $this->html[--$this->pos];
                    }
                }
            }
        }

        // end slash found
        if (($node->info[HDOM_INFO_SPACE][]=$this->copy_until_char('>'))=='/') {
            $node->info[HDOM_INFO_SLASH] = true;
            $node->info[HDOM_INFO_END] = $this->index-1;
        }
        else {
            if (!isset($this->tag_self_closing[strtolower($node->tag)]))
                $this->parent = $node;
        }

        // next
        if(++$this->pos<$this->size) $this->char = $this->html[$this->pos];
        return true;
    }

    // parse tag attributes
    private function parse_attr($node, $name) {
        $node->info[HDOM_INFO_SPACE][] = $this->copy_skip($this->token_blank);

        switch($this->char) {
            case '"':
                $node->info[HDOM_INFO_QUOTE][] = HDOM_QUOTE_DOUBLE;
                // next
                $this->char = $this->html[++$this->pos];
                $value = $this->copy_until_char('"');
                // next
                $this->char = $this->html[++$this->pos];
                break;
            case "'":
                $node->info[HDOM_INFO_QUOTE][] = HDOM_QUOTE_SINGLE;
                // next
                $this->char = $this->html[++$this->pos];
                $value = $this->copy_until_char("'");
                // next
                $this->char = $this->html[++$this->pos];
                break;
            default:
                $node->info[HDOM_INFO_QUOTE][] = HDOM_QUOTE_NO;
                $value = $this->copy_until($this->token_attr);
        }

        $node->attr[$this->restore_noise($name)] = $this->restore_noise($value);
    }

    private function skip($chars) {
        while ($this->pos<$this->size) {
            if (!isset($chars[$this->char])) return;
            // next
            $this->char = $this->html[++$this->pos];
        }
    }

    private function copy_skip($chars) {
        $ret = '';
        while ($this->pos<$this->size) {
            if (!isset($chars[$this->char])) return $ret;
            $ret .= $this->char;
            // next
            $this->char = $this->html[++$this->pos];
        }
        return $ret;
    }

    private function copy_until($chars) {
        $ret = '';
        while ($this->pos<$this->size) {
            if (isset($chars[$this->char])) return $ret;
            $ret .= $this->char;
            // next
            $this->char = $this->html[++$this->pos];
        }
        return $ret;
    }

    private function copy_until_char($char, $escape=true) {
        $ret = '';
        while ($this->char!=$char && $this->pos<$this->size) {
            if ($escape && $this->char=='\\') {
                $ret .= $this->char;
                // next
                $this->char = $this->html[++$this->pos];
            }
            $ret .= $this->char;
            // next
            if(++$this->pos<$this->size) $this->char = $this->html[$this->pos];
        }
        return $ret;
    }

    // remove noise from html content
    private function remove_noise($pattern, $is_tag_remove=true) {
        $count = preg_match_all($pattern, $this->html, $matches, PREG_SET_ORDER|PREG_OFFSET_CAPTURE);
        for ($i=$count-1; $i>-1; --$i) {
            $key = '___noise___'.sprintf("% 3d", count($this->noise));
            $idx = ($is_tag_remove) ? 0 : 1;
            $this->noise[$key] = $matches[$i][$idx][0];
            $this->html = substr_replace($this->html, $key, $matches[$i][$idx][1], strlen($matches[$i][$idx][0]));
        }
    }

    // restore noise to html content
    private function restore_noise($text) {
        while(($pos=strpos($text, '___noise___'))!==false) {
            $key = '___noise___'.$text[$pos+11].$text[$pos+12].$text[$pos+13];
            if (isset($this->noise[$key]))
                $text = substr($text, 0, $pos).$this->noise[$key].substr($text, $pos+14);
        }
        return $text;
    }
}
?>