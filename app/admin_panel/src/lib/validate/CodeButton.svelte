<!-- ``` -->
<script>
    import xmlFormat from "xml-formatter";
    import { behaviour_tree_xml_code } from "../../store/behaviour_tree_store";

    /** @type {string} */
    let validate_behaviour_tree_text = "Validate Behaviour Tree";

    /** @type {string} */
    let validate_behaviour_tree_color = "#4C9CFF";

    /** @type {boolean} */
    let validate_behaviour_tree_debounce = false;

    async function validateBehaviourTree() {
        if (validate_behaviour_tree_debounce) {
            return;
        }
        validate_behaviour_tree_debounce = true;
        try {
            validate_behaviour_tree_text = "Validating Behaviour Tree";
            validate_behaviour_tree_color = "#4C23FF";
            const result = JSON.parse(await api.validateBehaviourTree({
                data: xmlFormat.minify($behaviour_tree_xml_code),
            }));
            if (result.success) {
                validate_behaviour_tree_text = "The Behaviour Tree is valid!";
                validate_behaviour_tree_color = "#34A83B";
            } else {
                validate_behaviour_tree_text = result.message;
                validate_behaviour_tree_color = "#AA3434";
            }
        } catch (error) { 
                validate_behaviour_tree_text = error;
                validate_behaviour_tree_color = "#AA3434";
            }
        setTimeout(() => {
            validate_behaviour_tree_text = "Validate Behaviour Tree";
            validate_behaviour_tree_color = "#4C9CFF";
            validate_behaviour_tree_debounce = false;
        }, 1000);
    }
</script>

<button
    on:mousedown={validateBehaviourTree}
    class="p-2 rounded-lg shadow-lg relative inset-0"
    style="background-color: {validate_behaviour_tree_color}; color: white; width: 100%; border: none;">{validate_behaviour_tree_text}</button
>
<!-- ``` -->
