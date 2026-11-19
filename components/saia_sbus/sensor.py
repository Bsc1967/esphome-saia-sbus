import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import sensor, uart
from esphome.const import CONF_ID

DEPENDENCIES = ["uart"]
ns = cg.esphome_ns.namespace("saia_sbus")
SaiaSBus = ns.class_("SaiaSBus", cg.PollingComponent, uart.UARTDevice)
CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(SaiaSBus),
    cv.Required("direction_pin"): pins.gpio_output_pin_schema,
    cv.Optional("address", default=1): cv.int_range(min=0, max=254),
    cv.Required("energy"): sensor.sensor_schema(unit_of_measurement="kWh", accuracy_decimals=2, device_class="energy", state_class="total_increasing"),
    cv.Required("power"): sensor.sensor_schema(unit_of_measurement="W", accuracy_decimals=0, device_class="power", state_class="measurement"),
    cv.Required("voltage"): sensor.sensor_schema(unit_of_measurement="V", accuracy_decimals=0, device_class="voltage", state_class="measurement"),
    cv.Required("current"): sensor.sensor_schema(unit_of_measurement="A", accuracy_decimals=1, device_class="current", state_class="measurement"),
}).extend(cv.polling_component_schema("5s")).extend(uart.UART_DEVICE_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
    cg.add(var.set_direction_pin(await cg.gpio_pin_expression(config["direction_pin"])))
    cg.add(var.set_address(config["address"]))
    for key in ("energy", "power", "voltage", "current"):
        cg.add(getattr(var, "set_" + key)(await sensor.new_sensor(config[key])))
