import { hoverTooltip } from "@codemirror/view";

export const range_0_180 = [
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "10",
    "11",
    "12",
    "13",
    "14",
    "15",
    "16",
    "17",
    "18",
    "19",
    "20",
    "21",
    "22",
    "23",
    "24",
    "25",
    "26",
    "27",
    "28",
    "29",
    "30",
    "31",
    "32",
    "33",
    "34",
    "35",
    "36",
    "37",
    "38",
    "39",
    "40",
    "41",
    "42",
    "43",
    "44",
    "45",
    "46",
    "47",
    "48",
    "49",
    "50",
    "51",
    "52",
    "53",
    "54",
    "55",
    "56",
    "57",
    "58",
    "59",
    "60",
    "61",
    "62",
    "63",
    "64",
    "65",
    "66",
    "67",
    "68",
    "69",
    "70",
    "71",
    "72",
    "73",
    "74",
    "75",
    "76",
    "77",
    "78",
    "79",
    "80",
    "81",
    "82",
    "83",
    "84",
    "85",
    "86",
    "87",
    "88",
    "89",
    "90",
    "91",
    "92",
    "93",
    "94",
    "95",
    "96",
    "97",
    "98",
    "99",
    "100",
    "101",
    "102",
    "103",
    "104",
    "105",
    "106",
    "107",
    "108",
    "109",
    "110",
    "111",
    "112",
    "113",
    "114",
    "115",
    "116",
    "117",
    "118",
    "119",
    "120",
    "121",
    "122",
    "123",
    "124",
    "125",
    "126",
    "127",
    "128",
    "129",
    "130",
    "131",
    "132",
    "133",
    "134",
    "135",
    "136",
    "137",
    "138",
    "139",
    "140",
    "141",
    "142",
    "143",
    "144",
    "145",
    "146",
    "147",
    "148",
    "149",
    "150",
    "151",
    "152",
    "153",
    "154",
    "155",
    "156",
    "157",
    "158",
    "159",
    "160",
    "161",
    "162",
    "163",
    "164",
    "165",
    "166",
    "167",
    "168",
    "169",
    "170",
    "171",
    "172",
    "173",
    "174",
    "175",
    "176",
    "177",
    "178",
    "179",
    "180",
];

export const range_0_100 = [
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "10",
    "11",
    "12",
    "13",
    "14",
    "15",
    "16",
    "17",
    "18",
    "19",
    "20",
    "21",
    "22",
    "23",
    "24",
    "25",
    "26",
    "27",
    "28",
    "29",
    "30",
    "31",
    "32",
    "33",
    "34",
    "35",
    "36",
    "37",
    "38",
    "39",
    "40",
    "41",
    "42",
    "43",
    "44",
    "45",
    "46",
    "47",
    "48",
    "49",
    "50",
    "51",
    "52",
    "53",
    "54",
    "55",
    "56",
    "57",
    "58",
    "59",
    "60",
    "61",
    "62",
    "63",
    "64",
    "65",
    "66",
    "67",
    "68",
    "69",
    "70",
    "71",
    "72",
    "73",
    "74",
    "75",
    "76",
    "77",
    "78",
    "79",
    "80",
    "81",
    "82",
    "83",
    "84",
    "85",
    "86",
    "87",
    "88",
    "89",
    "90",
    "91",
    "92",
    "93",
    "94",
    "95",
    "96",
    "97",
    "98",
    "99",
    "100",
]

export const wheel_type = ["Left", "Right", "Both"]

export const servo_type = ["FrontWheels", "CameraServo1", "CameraServo2"]

export const common_nodes = [
    // Composite
    "Selector",
    "Sequence",

    // Decorator
    "Invert",
    "Repeat",

    // Leaf
    "Fail",
    "Succeed",
    "Task",
    "UseRoot",

    // Task Nodes
    "Action:PauseExecution",

    "Action:Print",

    "Action:SetAngle",
    "Action:SetSpeed",
    "Action:SetWheelDirection",

    "Condition:SuccessOnAverageNearbyScan",
];

export const xml_schema = {
    elements: [
        {
            name: "BehaviourTree",
            top: true,
            children: ["Root"],
            attributes: [
                "cycle_limit",
            ],
            completion: { type: "keyword" },
        },
        {
            name: "Root",
            attributes: ["id"],
            children: common_nodes,
            completion: { type: "keyword" },
        },
        // Composite
        {
            name: "Selector",
            children: common_nodes,
            completion: { type: "keyword" },
        },
        {
            name: "Sequence",
            children: common_nodes,
            completion: { type: "keyword" },
        },
        {
            name: "Random",
            children: common_nodes,
            completion: { type: "keyword" },
        },
        // Decorator
        {
            name: "Invert",
            children: common_nodes,
            completion: { type: "keyword" },
        },
        {
            name: "Repeat",
            children: common_nodes,
            attributes: [
                "count",
                {
                    name: "break_on_fail",
                    values: ["true", "false"],
                    completion: { type: "keyword" },
                }
            ],
            completion: { type: "keyword" },
        },

        // Leaf
        {
            name: "Fail",
            completion: { type: "keyword" },
        },
        {
            name: "Succeed",
            completion: { type: "keyword" },
        },
        {
            name: "UseRoot",
            attributes: ["id"],
            completion: { type: "keyword" },
        },
        {
            name: "Action:PauseExecution",
            attributes: ["ms"],
            completion: { type: "keyword" },
        },
        {
            name: "Action:Print",
            attributes: ["text"],
            completion: { type: "keyword" },
        },
        {
            name: "Action:SetAngle",
            attributes: [
                {
                    name: "servo_type",
                    values: servo_type,
                    completion: { type: "keyword" },
                },
                {
                    name: "angle",
                    values: range_0_180,
                    completion: { type: "keyword" },
                },
            ],
            completion: { type: "keyword" },
        },
        {
            name: "Action:SetSpeed",
            attributes: [
                {
                    name: "wheel_type",
                    values: wheel_type,
                    completion: { type: "keyword" },
                },
                {
                    name: "speed",
                    values: range_0_100,
                    completion: { type: "keyword" },
                },
            ],
            completion: { type: "keyword" },
        },
        {
            name: "Action:SetWheelDirection",
            attributes: [
                {
                    name: "wheel_type",
                    values: wheel_type,
                    completion: { type: "keyword" },
                },
                {
                    name: "direction_type",
                    values: ["Forward", "Backward"],
                    completion: { type: "keyword" },
                },
            ],
            completion: { type: "keyword" },
        },
        {
            name: "Condition:SuccessOnAverageNearbyScan",
            attributes: [
                {
                    name: "min_angle",
                    values: range_0_180,
                    completion: { type: "keyword" },
                },
                {
                    name: "max_angle",
                    values: range_0_180,
                    completion: { type: "keyword" },
                },
                "distance",
            ],
            completion: { type: "keyword" },
        },
    ],
    attributes: [{ name: "name", global: true }],
};

/**
 * @type {Map<string, string>}
 */
const node_information_list = new Map([
    ["Root", "Start of every node tree"],
    ["Selector", "Runs children until one succeeds"],
    ["Sequence", "Runs children until one fails"],

    ["UseRoot", "Goes to the given root node id"],
    ["Invert", "Inverts the result of the child"],
    ["Repeat", "Repeats the child given the amount of times"],

    ["Fail", "Always fails"],
    ["Succeed", "Always succeeds"],

    ["Action:PauseExecution", "Pauses the execution for the given amount of time"],
    ["Action:Print", "Prints the given message"],

    ["Action:SetAngle", "Turns the car by the given angle"],
    ["Action:SetSpeed", "Moves the car by the given speed"],
    ["Action:SetWheelDirection", "Sets the direction of the car"],

    ["Condition:SuccessOnAverageNearbyScan", "Checks if there are nearby points"],
]);

// Modified code from: https://codemirror.net/examples/tooltip/
export const node_hover = hoverTooltip((view, pos, side) => {
    const { from, to, text } = view.state.doc.lineAt(pos);
    let start = pos,
        end = pos;
    while (start > from && /[\w|:]/.test(text[start - from - 1])) start--;
    while (end < to && /[\w|:]/.test(text[end - from])) end++;
    if ((start == pos && side < 0) || (end == pos && side > 0)) return null;
    const highlighted_node = text.slice(start - from, end - from);
    const node_information = node_information_list.get(highlighted_node);
    if (!node_information) return null;
    return {
        pos: start,
        end,
        above: true,
        create(_) {
            const dom = document.createElement("div");
            dom.textContent = node_information;
            return { dom };
        },
    };
});