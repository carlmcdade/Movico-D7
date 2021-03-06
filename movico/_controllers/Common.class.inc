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

/*
 * This is loaded Singleton because there is no need to have more than one instance
 * and hooks should always be created from a single area. At present Drupal modules
 * can create hooks from anywhere which leads to confusion. But it is a Facade pattern
 * that wraps Drupals inner functions to prevent to protect the application from version 
 * differences
 *
 */
 
 /*
 *
 */

class Common{

    static private $_instance;
    private $data = array();
    private $ini_settings = array();
    private $update = FALSE;

    /*
     * @todo create a registry for modules and hooks
     */
    private $modules_list = array('SmartInventory','ConClass1','ConClass2','ConClass3','ConClass4','ConClass5');
    private $hooks_list = array('hook_links','hook_forms','hook_content','hook_blocks','hook_access');


    private function __construct()
    {
        
        //Please extend this class to gain extra functionality

        if(file_exists(INI_FILENAME))
        {
            /*
             * suppress the warning that comes with using serialized data
             * parse_ini_file() accepts and sends serialixzed data fine
             *
             * @todo Change this to PHP 5.3.0 a.s.a.p which will accept raw data
             * by setting the parameter
             *
             */
            $this->ini_settings =  @parse_ini_file(INI_FILENAME);
        }

    }

    function __destruct()
    {

        /*
         * update check point
         * @todo: Forgot that destructors do not respond to normal Exception
         * handling. There are work arounds for this though.
         *
         */
        if(!$this->update)
        {
            return;
        }

        try
        {
            //overwrite INI file with the version in memory
            
            $handle = fopen(INI_FILENAME, "w");
            foreach ($this->ini_settings as $key => $value)
            {
                if(isset($this->delete) && in_array($key, $this->delete))
                {
                    continue;
                }
                
                fwrite($handle, "$key = \"$value\"\n");
            }

            unset($this->delete);
            fclose($handle);

        }
        catch(Exception $e)
        {
            trigger_error('Could not write to file.' . INI_FILENAME);
        }

    }


    private function __clone()
    {
        throw new Exception('Please extend this Class to gain that functionality');
    }

    final public static function get_instance()
    {
        return isset(self::$_instance) ? self::$_instance : self::$_instance = new self();
    }


    function get_hooks($hook = NULL)
    {

        $output = array();

        foreach ($this->modules_list as $module)
        {
            
            
            // hook may not exist in all modules. Handle exception with 
            // continue
            
            try
            {
                $Class = new ReflectionClass($module);
                $Method = new ReflectionMethod($Class->getName(), $hook);
                $Instance = $Class->newInstance();
            }
            catch(Exception $e)
            {
                continue;
            }       

            /*
             * 
             * Static methods do not require a Class instance
             * Invoking a private method is not allowed. 
             * The exception is handled by ignoring the method and continuing on
             * 
             */

            if ($Method->isStatic())
            {
                $Instance = NULL;
            }
                
            try
            {
                $output = array_merge($output, $Method->invoke($Instance));
            }
            catch(Exception $e)
            {
                trigger_error('You are attempting to use a private method.');
                continue;
            }
             
        }

        $this->data = $output;
        return $this->data;
    }

    function get_all_hooks()
    {
        $output = array();

        foreach ($this->hooks_list as $hook)
        {
            $output[$hook] = $this->get_hooks($hook);
        }

        $this->data = $output;
        return $this->data;
    }

    public function var_get($name)
    {
        if(isset($this->ini_settings[$name]))
        {
            $output = unserialize($this->ini_settings[$name]);
            return $output;

        }
        else
        {
            return(NULL);
        }
    }

    public function var_set($name, $value)
    {
       ////update only on change
       if(!isset($this->ini_settings[$name]) || ($this->ini_settings[$name] != $value))
       {
           $this->ini_settings[$name] = serialize($value);
           $this->update = TRUE;
       }
       return;

    }

    /*
     * Sometimes you need or want to delete the variable completely
     *
     */
    function var_delete($name)
    {
        unset($this->ini_settings[$name]);
        $this->delete[] = $name;
        return;
    }

}

?>
