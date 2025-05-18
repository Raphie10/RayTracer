// Variables globales
let primitives = [];
let lights = [];

document.addEventListener('DOMContentLoaded', () => {
    initializeSectionButtons();
    initializeAddButtons();
    initializeCopyButton();
    initializeCameraInputs();
    updateConfigPreview();
});

function initializeSectionButtons() {
    const sectionButtons = document.querySelectorAll('.section-btn');
    const sections = document.querySelectorAll('.section');

    sectionButtons.forEach(button => {
        button.addEventListener('click', () => {
            const targetSection = button.getAttribute('data-section');
            sections.forEach(section => {
                if (section.id === `${targetSection}-section`) {
                    section.classList.remove('hidden');
                } else {
                    section.classList.add('hidden');
                }
            });
        });
    });
    document.getElementById('camera-section').classList.remove('hidden');
}

function initializeAddButtons() {
    document.getElementById('add-primitive').addEventListener('click', () => {
        const type = document.getElementById('primitive-type').value;
        addPrimitive(type);
    });
    document.getElementById('add-light').addEventListener('click', () => {
        const type = document.getElementById('light-type').value;
        addLight(type);
    });
}

function initializeGenerateButton() {
    document.getElementById('generate-config').addEventListener('click', generateConfig);
    document.querySelectorAll('input').forEach(input => {
        input.addEventListener('change', updateConfigPreview);
    });
}

function initializeCameraInputs() {
    // Ajouter des écouteurs d'événements à tous les champs de la caméra
    document.getElementById('camera-position-x').addEventListener('input', updateConfigPreview);
    document.getElementById('camera-position-y').addEventListener('input', updateConfigPreview);
    document.getElementById('camera-position-z').addEventListener('input', updateConfigPreview);
    document.getElementById('camera-rotation-x').addEventListener('input', updateConfigPreview);
    document.getElementById('camera-rotation-y').addEventListener('input', updateConfigPreview);
    document.getElementById('camera-rotation-z').addEventListener('input', updateConfigPreview);
    document.getElementById('camera-fov').addEventListener('input', updateConfigPreview);
    document.getElementById('camera-resolution-width').addEventListener('input', updateConfigPreview);
    document.getElementById('camera-resolution-height').addEventListener('input', updateConfigPreview);
}

function addPrimitive(type) {
    const id = Date.now();
    const primitive = {
        id,
        type,
        position: { x: 0, y: 0, z: 0 },
        rotation: { x: 0, y: 0, z: 0 },
        color: { r: 255, g: 255, b: 255 }
    };

    switch(type) {
        case 'sphere':
            primitive.radius = 1;
            break;
        case 'cylinder':
        case 'cone':
            primitive.radius = 1;
            primitive.height = 2;
            break;
        case 'donut':
            primitive.innerRadius = 0.5;
            primitive.outerRadius = 1;
            break;
        case 'objfile':
            primitive.filepath = "";
            break;
    }

    primitives.push(primitive);
    renderPrimitive(primitive);
    updateConfigPreview();
}

function renderPrimitive(primitive) {
    const primitivesListElement = document.getElementById('primitives-list');
    const primitiveElement = document.createElement('div');
    primitiveElement.className = 'primitive-item';
    primitiveElement.id = `primitive-${primitive.id}`;

    let html = `
        <button class="remove-btn" onclick="removePrimitive(${primitive.id})">×</button>
        <h3>${capitalizeFirstLetter(primitive.type)}</h3>
        <div class="form-group">
            <label>Position (x y z):</label>
            <div class="vector-inputs">
                <input type="number" step="0.1" value="${primitive.position.x}"
                       onchange="updatePrimitiveProperty(${primitive.id}, 'position', 'x', this.value)">
                <input type="number" step="0.1" value="${primitive.position.y}"
                       onchange="updatePrimitiveProperty(${primitive.id}, 'position', 'y', this.value)">
                <input type="number" step="0.1" value="${primitive.position.z}"
                       onchange="updatePrimitiveProperty(${primitive.id}, 'position', 'z', this.value)">
            </div>
        </div>
    `;
    // Ne pas afficher la rotation pour les sphères et les plans et les fichiers OBJ
    if (primitive.type !== 'sphere' && primitive.type !== 'plane' && primitive.type !== 'objfile') {
        html += `
        <div class="form-group">
            <label>Rotation (x y z):</label>
            <div class="vector-inputs">
                <input type="number" step="0.1" value="${primitive.rotation.x}"
                       onchange="updatePrimitiveProperty(${primitive.id}, 'rotation', 'x', this.value)">
                <input type="number" step="0.1" value="${primitive.rotation.y}"
                       onchange="updatePrimitiveProperty(${primitive.id}, 'rotation', 'y', this.value)">
                <input type="number" step="0.1" value="${primitive.rotation.z}"
                       onchange="updatePrimitiveProperty(${primitive.id}, 'rotation', 'z', this.value)">
            </div>
        </div>
        `;
    }

    html += `
        <div class="form-group">
            <label>Couleur:</label>
            <div class="color-picker-container">
                <input type="color" class="color-picker" 
                       value="#${rgbToHex(primitive.color.r, primitive.color.g, primitive.color.b)}"
                       onchange="updatePrimitiveColorFromPicker(${primitive.id}, this.value)">
                <div class="color-preview" style="background-color: rgb(${primitive.color.r}, ${primitive.color.g}, ${primitive.color.b})"></div>
            </div>
        </div>
    `;

    switch(primitive.type) {
        case 'sphere':
            html += `
                <div class="form-group">
                    <label>Rayon:</label>
                    <input type="number" min="0.1" step="0.1" value="${primitive.radius}"
                           onchange="updatePrimitiveProperty(${primitive.id}, 'radius', null, this.value)">
                </div>
            `;
            break;
        case 'cylinder':
        case 'cone':
            html += `
                <div class="form-group">
                    <label>Rayon:</label>
                    <input type="number" min="0.1" step="0.1" value="${primitive.radius}"
                           onchange="updatePrimitiveProperty(${primitive.id}, 'radius', null, this.value)">
                </div>
                <div class="form-group">
                    <label>Hauteur:</label>
                    <input type="number" min="0.1" step="0.1" value="${primitive.height}"
                           onchange="updatePrimitiveProperty(${primitive.id}, 'height', null, this.value)">
                </div>
            `;
            break;
        case 'donut':
            html += `
                <div class="form-group">
                    <label>Rayon interne:</label>
                    <input type="number" min="0.1" step="0.1" value="${primitive.innerRadius}"
                           onchange="updatePrimitiveProperty(${primitive.id}, 'innerRadius', null, this.value)">
                </div>
                <div class="form-group">
                    <label>Rayon externe:</label>
                    <input type="number" min="0.1" step="0.1" value="${primitive.outerRadius}"
                           onchange="updatePrimitiveProperty(${primitive.id}, 'outerRadius', null, this.value)">
                </div>
            `;
            break;
        case 'objfile':
            html += `
                <div class="form-group">
                    <label>Nom du fichier:</label>
                    <input type="text" value="${primitive.filepath}"
                           onchange="updatePrimitiveProperty(${primitive.id}, 'filepath', null, this.value)">
                    <span style="display: block; font-size: 0.8em; color: #666; margin-top: 5px;">model/<span style="font-weight: bold;">[nom]</span>.obj</span>
                </div>
            `;
            break;
    }
    primitiveElement.innerHTML = html;
    primitivesListElement.appendChild(primitiveElement);
}

function updatePrimitiveProperty(id, property, subProperty, value) {
    const primitive = primitives.find(p => p.id === id);
    if (!primitive) return;

    value = !isNaN(value) ? parseFloat(value) : value;

    if (subProperty) {
        primitive[property][subProperty] = value;
    } else {
        primitive[property] = value;
    }
    updateConfigPreview();
}

function removePrimitive(id) {
    primitives = primitives.filter(p => p.id !== id);
    const element = document.getElementById(`primitive-${id}`);
    if (element) element.remove();
    updateConfigPreview();
}

function addLight(type) {
    const id = Date.now();
    const light = {
        id,
        type,
        color: { r: 255, g: 255, b: 255 },
        intensity: 1
    };

    if (type === 'point') {
        light.position = { x: 0, y: 0, z: 0 };
    } else {
        light.direction = { x: 0, y: 0, z: 0 };
    }

    lights.push(light);
    renderLight(light);
    updateConfigPreview();
}

function renderLight(light) {
    const lightsListElement = document.getElementById('lights-list');
    const lightElement = document.createElement('div');
    lightElement.className = 'light-item';
    lightElement.id = `light-${light.id}`;

    let html = `
        <button class="remove-btn" onclick="removeLight(${light.id})">×</button>
        <h3>Lumière ${light.type === 'point' ? 'ponctuelle' : 'directionnelle'}</h3>
        <div class="form-group">
            <label>Couleur:</label>
            <div class="color-picker-container">
                <input type="color" class="color-picker" 
                       value="#${rgbToHex(light.color.r, light.color.g, light.color.b)}"
                       onchange="updateLightColorFromPicker(${light.id}, this.value)">
                <div class="color-preview" style="background-color: rgb(${light.color.r}, ${light.color.g}, ${light.color.b})"></div>
            </div>
        </div>
        <div class="form-group">
            <label>Intensité:</label>
            <input type="number" min="0" step="0.1" value="${light.intensity}"
                   onchange="updateLightProperty(${light.id}, 'intensity', null, this.value)">
        </div>
    `;

    if (light.type === 'point') {
        html += `
            <div class="form-group">
                <label>Position (x y z):</label>
                <div class="vector-inputs">
                    <input type="number" step="0.1" value="${light.position.x}"
                           onchange="updateLightProperty(${light.id}, 'position', 'x', this.value)">
                    <input type="number" step="0.1" value="${light.position.y}"
                           onchange="updateLightProperty(${light.id}, 'position', 'y', this.value)">
                    <input type="number" step="0.1" value="${light.position.z}"
                           onchange="updateLightProperty(${light.id}, 'position', 'z', this.value)">
                </div>
            </div>
        `;
    } else {
        html += `
            <div class="form-group">
                <label>Direction (x y z):</label>
                <div class="vector-inputs">
                    <input type="number" step="0.1" value="${light.direction.x}"
                           onchange="updateLightProperty(${light.id}, 'direction', 'x', this.value)">
                    <input type="number" step="0.1" value="${light.direction.y}"
                           onchange="updateLightProperty(${light.id}, 'direction', 'y', this.value)">
                    <input type="number" step="0.1" value="${light.direction.z}"
                           onchange="updateLightProperty(${light.id}, 'direction', 'z', this.value)">
                </div>
            </div>
        `;
    }
    lightElement.innerHTML = html;
    lightsListElement.appendChild(lightElement);
}

function updateLightProperty(id, property, subProperty, value) {
    const light = lights.find(l => l.id === id);
    if (!light) return;

    value = !isNaN(value) ? parseFloat(value) : value;

    if (subProperty) {
        light[property][subProperty] = value;
    } else {
        light[property] = value;
    }
    updateConfigPreview();
}

function removeLight(id) {
    lights = lights.filter(l => l.id !== id);
    const element = document.getElementById(`light-${id}`);
    if (element) element.remove();
    updateConfigPreview();
}

function updateConfigPreview() {
    const configPreview = document.getElementById('config-preview');
    let configText = '';

    const posX = document.getElementById('camera-position-x').value;
    const posY = document.getElementById('camera-position-y').value;
    const posZ = document.getElementById('camera-position-z').value;
    const rotX = document.getElementById('camera-rotation-x').value;
    const rotY = document.getElementById('camera-rotation-y').value;
    const rotZ = document.getElementById('camera-rotation-z').value;
    const fov = document.getElementById('camera-fov').value;
    const resWidth = document.getElementById('camera-resolution-width').value;
    const resHeight = document.getElementById('camera-resolution-height').value;
    const toFloat = (val) => {
        const num = parseFloat(val);
        return Number.isInteger(num) ? num + ".0" : num;
    };

    configText += `camera = {\n`;
    configText += `    fieldOfView = ${toFloat(fov)};\n`;
    configText += `    position = { x = ${toFloat(posX)}; y = ${toFloat(posY)}; z = ${toFloat(posZ)}; };\n`;
    configText += `    rotation = { x = ${(rotX)}; y = ${(rotY)}; z = ${(rotZ)}; };\n`;
    configText += `    resolution = {\n`;
    configText += `        width = ${resWidth};\n`;
    configText += `        height = ${resHeight};\n`;
    configText += `    };\n`;
    configText += `};\n\n`;

    if (primitives.length > 0) {
        configText += `primitives = {\n`;

        const spheres = primitives.filter(p => p.type === 'sphere');
        const planes = primitives.filter(p => p.type === 'plane');
        const cylinders = primitives.filter(p => p.type === 'cylinder');
        const cones = primitives.filter(p => p.type === 'cone');
        const donuts = primitives.filter(p => p.type === 'donut');
        const objfile = primitives.filter(p => p.type === 'objfile');

        // Sphères
        if (spheres.length > 0) {
            configText += `    spheres = (\n`;
            spheres.forEach((sphere, index) => {
                configText += `        {\n`;
                configText += `            x = ${toFloat(sphere.position.x)}; y = ${toFloat(sphere.position.y)}; z = ${toFloat(sphere.position.z)}; r = ${toFloat(sphere.radius)};\n`;
                configText += `            color = { r = ${sphere.color.r}; g = ${sphere.color.g}; b = ${sphere.color.b}; };\n`;
                configText += `        }${index < spheres.length - 1 ? ',' : ''}\n`;
            });
            configText += `    );\n`;
        }
        // Plans
        if (planes.length > 0) {
            configText += `    planes = (\n`;
            planes.forEach((plane, index) => {
                configText += `        {\n`;
                const posValues = [Math.abs(plane.position.x), Math.abs(plane.position.y), Math.abs(plane.position.z)];
                const maxIndex = posValues.indexOf(Math.max(...posValues));
                const axis = maxIndex === 0 ? "X" : maxIndex === 1 ? "Y" : "Z";
                configText += `            axis = "${axis}";\n`;
                configText += `            position = ${toFloat(plane.position[axis.toLowerCase()])};\n`;
                configText += `            color = { r = ${plane.color.r}; g = ${plane.color.g}; b = ${plane.color.b}; };\n`;
                configText += `        }${index < planes.length - 1 ? ',' : ''}\n`;
            });
            configText += `    );\n`;
        }
        // Cylindres
        if (cylinders.length > 0) {
            configText += `    cylinders = (\n`;
            cylinders.forEach((cylinder, index) => {
                configText += `        {\n`;
                configText += `            position = { x = ${toFloat(cylinder.position.x)}; y = ${toFloat(cylinder.position.y)}; z = ${toFloat(cylinder.position.z)}; };\n`;
                configText += `            radius = ${toFloat(cylinder.radius)};\n`;
                configText += `            height = ${toFloat(cylinder.height)};\n`;
                configText += `            color = { r = ${cylinder.color.r}; g = ${cylinder.color.g}; b = ${cylinder.color.b}; };\n`;
                configText += `            direction = { x = ${toFloat(cylinder.rotation.x)}; y = ${toFloat(cylinder.rotation.y)}; z = ${toFloat(cylinder.rotation.z)}; };\n`;
                configText += `        }${index < cylinders.length - 1 ? ',' : ''}\n`;
            });
            configText += `    );\n`;
        }
        // Cônes
        if (cones.length > 0) {
            configText += `    cones = (\n`;
            cones.forEach((cone, index) => {
                configText += `        {\n`;
                configText += `            position = { x = ${toFloat(cone.position.x)}; y = ${toFloat(cone.position.y)}; z = ${toFloat(cone.position.z)}; };\n`;
                configText += `            radius = ${toFloat(cone.radius)};\n`;
                configText += `            height = ${toFloat(cone.height)};\n`;
                configText += `            color = { r = ${cone.color.r}; g = ${cone.color.g}; b = ${cone.color.b}; };\n`;
                configText += `            direction = { x = ${toFloat(cone.rotation.x)}; y = ${toFloat(cone.rotation.y)}; z = ${toFloat(cone.rotation.z)}; };\n`;
                configText += `        }${index < cones.length - 1 ? ',' : ''}\n`;
            });
            configText += `    );\n`;
        }
        // Donuts
        if (donuts.length > 0) {
            configText += `    donuts = (\n`;
            donuts.forEach((donut, index) => {
                configText += `        {\n`;
                configText += `            position = { x = ${toFloat(donut.position.x)}; y = ${toFloat(donut.position.y)}; z = ${toFloat(donut.position.z)}; };\n`;
                configText += `            major_radius = ${toFloat(donut.outerRadius)};\n`;
                configText += `            minor_radius = ${toFloat(donut.innerRadius)};\n`;
                configText += `            color = { r = ${donut.color.r}; g = ${donut.color.g}; b = ${donut.color.b}; };\n`;
                configText += `            direction = { x = ${toFloat(donut.rotation.x)}; y = ${toFloat(donut.rotation.y)}; z = ${toFloat(donut.rotation.z)}; };\n`;
                configText += `        }${index < donuts.length - 1 ? ',' : ''}\n`;
            });
            configText += `    );\n`;
        }
        // Fichiers OBJ
        if (objfile.length > 0) {
            configText += `    objfiles = (\n`;
            objfile.forEach((obj, index) => {
                configText += `        {\n`;
                configText += `            filename = "model/${obj.filepath}.obj";\n`;
                configText += `            color = { r = ${obj.color.r}; g = ${obj.color.g}; b = ${obj.color.b}; };\n`;
                configText += `            position = { x = ${toFloat(obj.position.x)}; y = ${toFloat(obj.position.y)}; z = ${toFloat(obj.position.z)}; };\n`;
                configText += `        }${index < objfile.length - 1 ? ',' : ''}\n`;
            });
            configText += `    );\n`;
        }
        configText += `};\n\n`;
    }
    // Lights config
    if (lights.length > 0) {
        configText += `lights = {\n`;
        const directionalLights = lights.filter(l => l.type === 'directional');
        const pointLights = lights.filter(l => l.type === 'point');
        // Lumières directionnelles
        if (directionalLights.length > 0) {
            configText += `    directional = (\n`;
            directionalLights.forEach((light, index) => {
                configText += `        {\n`;
                configText += `            direction = {\n`;
                configText += `                x = ${light.direction.x};\n`;
                configText += `                y = ${light.direction.y};\n`;
                configText += `                z = ${light.direction.z};\n`;
                configText += `            };\n`;
                configText += `            intensity = ${toFloat(light.intensity)};\n`;
                configText += `            color = { r = ${light.color.r}; g = ${light.color.g}; b = ${light.color.b}; };\n`;
                configText += `        }${index < directionalLights.length - 1 ? ',' : ''}\n`;
            });
            configText += `    );\n`;
        }
        // Lumières ponctuelles
        if (pointLights.length > 0) {
            configText += `    point = (\n`;
            pointLights.forEach((light, index) => {
                configText += `        {\n`;
                configText += `            position = { x = ${light.position.x}; y = ${light.position.y}; z = ${light.position.z}; };\n`;
                configText += `            intensity = ${toFloat(light.intensity)};\n`;
                configText += `            color = { r = ${light.color.r}; g = ${light.color.g}; b = ${light.color.b}; };\n`;
                configText += `        }${index < pointLights.length - 1 ? ',' : ''}\n`;
            });
            configText += `    );\n`;
        }
        configText += `};\n`;
    }
    configPreview.textContent = configText;
}

function initializeCopyButton() {
    const copyConfigButton = document.getElementById('copy-config');
    if (!copyConfigButton) return;
    
    copyConfigButton.addEventListener('click', () => {
        const configContent = document.getElementById('config-preview').textContent;
        
        // Copier le texte dans le presse-papiers
        navigator.clipboard.writeText(configContent).then(() => {
            // Effet visuel pour indiquer que la copie a réussi
            const copyBtn = document.querySelector('.copy-btn');
            copyBtn.classList.add('copy-success');
            copyBtn.querySelector('.copy-icon').textContent = '✓';
            
            // Remettre l'icône d'origine après un délai
            setTimeout(() => {
                copyBtn.classList.remove('copy-success');
                copyBtn.querySelector('.copy-icon').textContent = '📋';
            }, 2000);
        }).catch(err => {
            console.error('Erreur lors de la copie: ', err);
            alert('Impossible de copier le texte. Votre navigateur ne semble pas prendre en charge cette fonctionnalité.');
        });
    });
}

function generateConfig() {
    // Cette fonction n'est plus utilisée mais conservée pour référence
    const configPreview = document.getElementById('config-preview').textContent;
    const configName = document.getElementById('config-name').value;
    if (!configName) {
        showMessage("Veuillez entrer un nom pour le fichier de configuration.", "error");
        return;
    }
    const element = document.createElement('a');
    const file = new Blob([configPreview], {type: 'text/plain'});
    element.href = URL.createObjectURL(file);
    element.download = `${configName}.cfg`;
    document.body.appendChild(element);
    element.click();
    document.body.removeChild(element);
    showMessage(`Configuration "${configName}.cfg" générée avec succès !`, "success");
}

function showMessage(message, type) {
    const messageElement = document.getElementById('generation-message');
    messageElement.textContent = message;
    messageElement.className = type === 'success' ? 'success-message' : 'error-message';

    setTimeout(() => {
        messageElement.textContent = '';
        messageElement.className = '';
    }, 3000);
}

function capitalizeFirstLetter(string) {
    return string.charAt(0).toUpperCase() + string.slice(1);
}

// Fonctions de conversion de couleurs
function componentToHex(c) {
    const hex = Math.round(c).toString(16);
    return hex.length === 1 ? "0" + hex : hex;
}

function rgbToHex(r, g, b) {
    return componentToHex(r) + componentToHex(g) + componentToHex(b);
}

function hexToRgb(hex) {
    const result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
    return result ? {
        r: parseInt(result[1], 16),
        g: parseInt(result[2], 16),
        b: parseInt(result[3], 16)
    } : { r: 0, g: 0, b: 0 };
}

function updatePrimitiveColorFromPicker(id, hexColor) {
    const primitive = primitives.find(p => p.id === id);
    if (!primitive) return;
    
    const rgb = hexToRgb(hexColor);
    primitive.color.r = rgb.r;
    primitive.color.g = rgb.g;
    primitive.color.b = rgb.b;
    
    // Mettre à jour la prévisualisation de couleur
    const colorPreview = document.querySelector(`#primitive-${id} .color-preview`);
    if (colorPreview) {
        colorPreview.style.backgroundColor = `rgb(${rgb.r}, ${rgb.g}, ${rgb.b})`;
    }
    
    updateConfigPreview();
}

function updateLightColorFromPicker(id, hexColor) {
    const light = lights.find(l => l.id === id);
    if (!light) return;
    
    const rgb = hexToRgb(hexColor);
    light.color.r = rgb.r;
    light.color.g = rgb.g;
    light.color.b = rgb.b;
    
    // Mettre à jour la prévisualisation de couleur
    const colorPreview = document.querySelector(`#light-${id} .color-preview`);
    if (colorPreview) {
        colorPreview.style.backgroundColor = `rgb(${rgb.r}, ${rgb.g}, ${rgb.b})`;
    }
    
    updateConfigPreview();
}

window.updatePrimitiveProperty = updatePrimitiveProperty;
window.removePrimitive = removePrimitive;
window.updateLightProperty = updateLightProperty;
window.removeLight = removeLight;
window.updatePrimitiveColorFromPicker = updatePrimitiveColorFromPicker;
window.updateLightColorFromPicker = updateLightColorFromPicker;
