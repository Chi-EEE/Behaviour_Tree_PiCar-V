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
            const result = await api.validateBehaviourTree({
                data: xmlFormat.minify($behaviour_tree_xml_code),
            });
            console.log(result);
            validate_behaviour_tree_text = "Sent Behaviour Tree!";
            validate_behaviour_tree_color = "#3457AA";
        } catch (error) {
            console.log(error)
            validate_behaviour_tree_text = "Unable to send Behaviour Tree!";
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
