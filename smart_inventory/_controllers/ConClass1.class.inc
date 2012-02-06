<?php

/*
 * @author Carl McDade
 * @copyright Copyright (c) 2010, Carl McDade
 * @since 2010-02-19
 * @version 0.2
 * @license http://opensource.org/licenses/gpl-license.php GNU Public License
 * @link http://www.drupal.se Drupal Sweden
 *
 */


class ConClass1{
	
	private $common;


    function __construct()
    {
        //Test the Common settings
        $this->common = Common::get_instance();
    }

    function hook_links()
    {
        $var['one1'] = l('class_one(1)', APP_MODULE . '/ConClass1/action_one');
        $var['two2'] = l('class_one(2)', APP_MODULE . '/ConClass1/action_two');

        return $var;
    }
    
    function hook_access()
    {
        $authorize = array(
			'ConClass1/action_one' => array('access foo'),
			'ConClass1/action_two' => array('access baz'),
			'ConClass1/action_three' => array('access boo','access suisse'),
        );

        return $authorize;
    }

    function hook_forms()
    {
        $var['sho'] = l('form element','app/helloworld/hello_log/update');
        $var['foo'] = l('form element','app/helloworld/hello_log/create');

        return $var;
    }

    function action_one()
    {
        $output = implode(' | ', $this->common->get_hooks('hook_links'));
        $output .= '<br />';
    	$output .= 'class of "' . get_class($this) . '" calling a method "' . __METHOD__ . '" in this class';
    	$variables['content'] = $output;

        return mvc_view('default', $variables);
        
    }

    function action_two()
    {
        //some variables to load into the view will appear as $[variable name]
        $variables['start'] = '<div style="border:1px solid #eaeaea">start</div>'.mvc_access(__function__);
        $variables['my_array'] = array('one','ton','son');
        $variables['end'] = '<div style="border:1px solid #eaeaea">end</div>';
    	
    	$output = implode(' | ', $this->common->get_hooks('hook_links'));
        $output .= '<br />';
    	$output .= 'class of "' . get_class($this) . '" calling a method "' . __METHOD__ . '" in this class';
    	$variables['content'] = $output;

        return mvc_view('default', $variables);
    }
    
    function action_three()
    {
        //some variables to load into the view will appear as $[variable name]
        $variables['start'] = '<div style="border:1px solid #eaeaea">start</div>'.mvc_access(__function__);
        $variables['my_array'] = array('one','ton','son');
        $variables['end'] = '<div style="border:1px solid #eaeaea">end</div>';
    	
    	$output = implode(' | ', $this->common->get_hooks('hook_links'));
        $output .= '<br />';
    	$output .= 'class of "' . get_class($this) . '" calling a method "' . __METHOD__ . '" in this class';
    	$variables['content'] = $output;

        return mvc_view('default', $variables);
    }
}

?>
